// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/CachedConnection/KLDebugNetworkingCachedConnectionBase.h"

#include "Log/KLDebugNetworkingLog.h"
#include "Message/Common/KLDebugNetworkingMessage_CheckIfAlive.h"
#include "Message/Header/KLDebugNetworkingMessage_Header.h"
#include "Message/Helpers/KLDebugNetworkingMessageHelpers.h"

// engine
#include "Containers/ArrayView.h"
#include "Math/UnrealMathUtility.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "SocketSubsystem.h"

#if DO_ENSURE
// engine
#include "Math/NumericLimits.h"
#endif

FKLDebugNetworkingCachedConnectionBase::FKLDebugNetworkingCachedConnectionBase(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _Socket)
    : mSocket(&_Socket)
{
    checkf(_ReadBufferSize > 0 && _WriteBufferSize > 0, TEXT("buffers must be bigger then 0"));
    mInitialReadBufferSize = static_cast<uint32>(_ReadBufferSize);

    mWriteBuffers.Reserve(_WriteBufferSize);
    mReceiveBuffer.Init(0, _ReadBufferSize);
    mTempMessageBuffer.Reserve(500);

    KL::Debug::Networking::Message::InitHeaderSize();

    mCheckIfAliveMessage.Reserve(50);
    FMemoryWriter Writer(mCheckIfAliveMessage);
    FKLDebugNetworkingMessage_CheckIfAlive Message;
    Message.Serialize(Writer);

    mLastTimeConnectionUsed = FPlatformTime::Seconds();
}

FKLDebugNetworkingCachedConnectionBase::~FKLDebugNetworkingCachedConnectionBase()
{
    Shutdown();
}

void FKLDebugNetworkingCachedConnectionBase::Shutdown()
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    check(SocketSubsystem);
    ClearSocket(*SocketSubsystem);
}

void FKLDebugNetworkingCachedConnectionBase::ClearSocket(ISocketSubsystem& _SocketSubsystem)
{
    if (mSocket)
    {
        mSocket->Close();
        _SocketSubsystem.DestroySocket(mSocket);
        mSocket = nullptr;
    }
}

void FKLDebugNetworkingCachedConnectionBase::Tick()
{
    /*
     * First check the status of the socket connection(if we are still connected or the server closed the connection)
     * Then read the data from the socket and place it in out array of bytes. Note that the array can contain data from a previous
     * iteration as we are waiting for a message to receive all data that it expects
     * After that we tick the child class first to see if it needs to read messages from the socket. If so the class will move
     * forward the read buffer index or let us know that the data currently in the buffer is not yet enough for the message is waiting
     * so no need to read more
     * Finally we perform the step in which we need to lock the buffers. First, if the socket buffer has data available, we read from it
     * After we try to send data if any
     */

    if (!mSocket)
    {
        return;
    }

    bool ShouldShutdown = CheckConnectionStatus();
    if (ShouldShutdown)
    {
        Shutdown();
        return;
    }

    if (mPendingMessages.IsEmpty())
    {
        mHasNewReadData = false;
    }

    if (mSocket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
    {
        const int32 BytesRead = TickReadConnectionData();
        const bool HasReadBytes = BytesRead > 0;
        const uint32 TotalBufferUsage = mReadBufferStartIndex + static_cast<uint32>(BytesRead);
        const TArrayView<uint8> ReadBufferView(mReceiveBuffer.GetData(), TotalBufferUsage);
        FMemoryReaderView MemReaderView(ReadBufferView);

        ensureMsgf(static_cast<uint32>(MemReaderView.TotalSize()) == TotalBufferUsage, TEXT("total size must match"));

        ShouldShutdown = TickReadWriteBuffers(HasReadBytes, MemReaderView);
        if (ShouldShutdown)
        {
            Shutdown();
            return;
        }
    }

    ShouldShutdown = TickChild();
    if (ShouldShutdown)
    {
        Shutdown();
    }
}

bool FKLDebugNetworkingCachedConnectionBase::CheckConnectionStatus()
{
    FSocket& Socket = GetSocketMutable();
    switch (Socket.GetConnectionState())
    {
    case ESocketConnectionState::SCS_Connected:
        mPendingConnectionTime = FPlatformTime::Seconds();
        break;
    case ESocketConnectionState::SCS_NotConnected:
        if (mPendingConnectionTime == -1.0)
        {
            mPendingConnectionTime = FPlatformTime::Seconds();
        }
        else
        {
            const double TimeSpan = FPlatformTime::Seconds() - mPendingConnectionTime;
            if (TimeSpan > 4.f)
            {
                return true;
            }
        }
        break;
    case ESocketConnectionState::SCS_ConnectionError:
        return true;
    }

    return false;
}

int32 FKLDebugNetworkingCachedConnectionBase::TickReadConnectionData()
{
    FSocket& Socket = GetSocketMutable();
    uint32 Size = 0;
    int32 BytesRead = 0;

    if (mReadBufferStartIndex == 0 && mReceiveBuffer.Num() > static_cast<int32>(mInitialReadBufferSize))
    {
        // resize the array to its original size so we dont waste too much space
        mReceiveBuffer.SetNum(mInitialReadBufferSize, true);
    }

    while (Socket.HasPendingData(Size) && Size > 0)
    {
        checkf(mReceiveBuffer.IsValidIndex(mReadBufferStartIndex), TEXT("must be valid"));
        const int32 SpaceLeftInBuffer = mReceiveBuffer.Max() - mReadBufferStartIndex;
        if (Size > static_cast<uint32>(SpaceLeftInBuffer))
        {
            ensureMsgf(false, TEXT("resizing array should not happen"));
            const int32 NewSize = Size - SpaceLeftInBuffer;
            mReceiveBuffer.SetNum(NewSize, false);
        }

        const uint32 MaxReadSize = FMath::Min(Size, static_cast<uint32>(mReceiveBuffer.Num() - mReadBufferStartIndex));
        Socket.Recv(&mReceiveBuffer[mReadBufferStartIndex], MaxReadSize, BytesRead);
        if (BytesRead == 0)
        {
            UE_LOG(LogKLDebug_Networking, Warning, TEXT("FKLDebugNetworkingCachedConnectionBase::TickReadConnectionData>> Socket had pending data but didnt read any bytes"));
            continue;
        }

        mLastTimeConnectionUsed = FPlatformTime::Seconds();
    }

    ensureMsgf(mReceiveBuffer.Max() == mReceiveBuffer.Num(), TEXT("num must point to the maximum value"));
    return BytesRead;
}

bool FKLDebugNetworkingCachedConnectionBase::TickReadWriteBuffers(const bool _HasReadBytes, FArchive& _Reader)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugNetworkingCachedConnectionBase_TickReadWriteBuffers);

    const bool ShouldShutdown = _HasReadBytes ? TickReadMessagesWithHeader(_Reader) : false;
    if (ShouldShutdown)
    {
        return true;
    }

    TickWriteConnectionData();
    if (mStartTimeFailToSendMessage > 0.0 && FPlatformTime::Seconds() - mStartTimeFailToSendMessage > 5.f)
    {
        UE_LOG(LogKLDebug_Networking, Warning, TEXT("FKLDebugNetworkingCachedConnectionBase::TickReadWriteBuffers>> Closing socket because we cant send data after 5 seconds of retries"));
        return true;
    }

    return false;
}

bool FKLDebugNetworkingCachedConnectionBase::TickReadMessagesWithHeader(FArchive& _Reader)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPCachedConnectionBase_TickReadMessagesWithHeader);

    if (_Reader.AtEnd())
    {
        return false;
    }

    TArray<FKLDebugNetworkingPendingMessage>& ReadBuffer = mPendingMessages;

    const int64 TotalSize = _Reader.TotalSize();
    int64 CurrentPosition = _Reader.Tell();

    while (!_Reader.AtEnd())
    {
        const int64 HeaderCurrentPosition = _Reader.Tell();
        if (TotalSize - HeaderCurrentPosition < KL::Debug::Networking::Message::GetHeaderSize())
        {
            // not enough data for the header
            break;
        }

        const FKLDebugNetworkingMessage_Header HeaderMessage{ _Reader };
        if (!HeaderMessage.IsValid())
        {
            // garbage in the stream skip one byte
            _Reader.Seek(HeaderCurrentPosition + 1);
            CurrentPosition++;
            continue;
        }

        const int64 CurrentReadBufferPosition = _Reader.Tell();
        const int64 RemainingSpace = TotalSize - CurrentReadBufferPosition;
        if (RemainingSpace < HeaderMessage.GetMessageDataSize())
        {
            break;
        }

        mTempMessageBuffer.SetNum(static_cast<int32>(HeaderMessage.GetMessageDataSize()), false);
        if (HeaderMessage.GetMessageDataSize() != 0)
        {
            _Reader.Serialize(mTempMessageBuffer.GetData(), HeaderMessage.GetMessageDataSize());
            CurrentPosition = _Reader.Tell();
        }

        FMemoryReader ChildClassMemoryReader(mTempMessageBuffer);
        if (HandleInternalMessages(HeaderMessage, ChildClassMemoryReader))
        {
            continue;
        }

        if (ReadBufferChildHasHandleMessage(HeaderMessage, ChildClassMemoryReader))
        {
            // this message was intended for the child class
            continue;
        }

        if (!HeaderMessage.IsSplittedMessage())
        {
            FKLDebugNetworkingPendingMessage& LastMessage = ReadBuffer.Emplace_GetRef(HeaderMessage, mTempMessageBuffer);
            if (!LastMessage.HasData())
            {
                ReadBuffer.RemoveAtSwap(ReadBuffer.Num() - 1, 1, false);
            }

            continue;
        }

        const int32 Index = mPendingSplittedMessages.IndexOfByKey(HeaderMessage.GetMessageID());
        if (Index == INDEX_NONE)
        {
            mPendingSplittedMessages.Emplace(HeaderMessage, mTempMessageBuffer);
            continue;
        }

        FKLDebugNetworkingPendingSplittedMessage& PendingMessage = mPendingSplittedMessages[Index];
        PendingMessage.AddData(mTempMessageBuffer, HeaderMessage.GetMessageDataOffset());
        if (!PendingMessage.IsFullyReceived())
        {
            continue;
        }

        if (!PendingMessage.HasFailedToReadData())
        {
            FKLDebugNetworkingPendingMessage& LastMessage = ReadBuffer.Emplace_GetRef(PendingMessage.GetHeaderMessage(), MoveTemp(PendingMessage.GetDataMutable()));
            if (!LastMessage.HasData())
            {
                ReadBuffer.RemoveAtSwap(ReadBuffer.Num() - 1, 1, false);
            }
        }

        mPendingSplittedMessages.RemoveAtSwap(Index, 1, false);
    }

    checkf(CurrentPosition < TNumericLimits<uint32>::Max(), TEXT("current position too high not expected"));
    checkf(TotalSize < TNumericLimits<uint32>::Max(), TEXT("total size too high not expected"));
    UpdateBufferStartIndex(CurrentPosition, TotalSize);

    if (!ReadBuffer.IsEmpty())
    {
        mHasNewReadData = true;
    }

    return false;
}

void FKLDebugNetworkingCachedConnectionBase::TickWriteConnectionData()
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugNetworkingCachedConnectionBase_TickWriteConnectionData);

    TArray<uint8>& WriteBuffer = GetWriteBuffer();
    if (WriteBuffer.Num() != WriteBuffer.Max())
    {
        FMemoryWriter Writer(WriteBuffer, false, true);
        TickChildWriteBuffer(Writer);
    }

    const double TimeSpan = FPlatformTime::Seconds() - mLastTimeConnectionUsed;
    if (WriteBuffer.IsEmpty() && TimeSpan > 8.f)
    {
        // if there is a lot of inactivity send a fake message to check if the connection is still valid or no
        FMemoryWriter Writer(WriteBuffer, false, true);
        const FKLDebugNetworkingMessage_CheckIfAlive Message;
        KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(Message, mCheckIfAliveMessage, Writer);
    }

    if (WriteBuffer.IsEmpty())
    {
        return;
    }

    SendData(WriteBuffer);
}

bool FKLDebugNetworkingCachedConnectionBase::HandleInternalMessages(const FKLDebugNetworkingMessage_Header& _Header, FArchive& _Data) const
{
    if (_Header.GetMessageEnumType() != static_cast<uint16>(EKLDebugNetworkingMessagesTypes::NetworkingRuntime))
    {
        return false;
    }

    return _Header.GetMessageType() == static_cast<uint16>(EKLDebugNetworkingMessage::IsConnectionAlive);
}

void FKLDebugNetworkingCachedConnectionBase::SendData(TArray<uint8>& _Buffer)
{
    mLastTimeConnectionUsed = FPlatformTime::Seconds();

    int32 BytesSent = 0;
    FSocket& Socket = GetSocketMutable();
    if (!Socket.Send(_Buffer.GetData(), _Buffer.Num(), BytesSent))
    {
        if (mStartTimeFailToSendMessage < 0.0)
        {
            mStartTimeFailToSendMessage = FPlatformTime::Seconds();
        }

        return;
    }

    mStartTimeFailToSendMessage = -1.0;
    const bool DataSentFully = BytesSent == _Buffer.Num();
    OnSendDataResultChild(DataSentFully);
    if (DataSentFully)
    {
        _Buffer.Reset();
        return;
    }

    UE_LOG(LogKLDebug_Networking, Warning, TEXT("FKLDebugNetworkingCachedConnectionBase::SendData>> Supposed to send [%d] but we sent just [%d]"), _Buffer.Num(), BytesSent);

    // remove the byts we managed to send
    _Buffer.RemoveAt(0, BytesSent, false);
}

void FKLDebugNetworkingCachedConnectionBase::UpdateBufferStartIndex(const uint32 _BufferStopPosition, const uint32 _TotalBufferSize)
{
    if (_BufferStopPosition == 0)
    {
        // nothing read so all the buffer needs to be kept
        mReadBufferStartIndex = _TotalBufferSize;
    }
    else if (_BufferStopPosition >= _TotalBufferSize)
    {
        // we read all the buffer so we can read from position 0 next time and fill it all up again
        mReadBufferStartIndex = 0;
    }
    else
    {
        // we read part of the buffer and we stopped at Curren_BufferStopPositiontPosition. So free up all previous bytes but keep
        // the ones we still need to process
        mReceiveBuffer.RemoveAt(0, _BufferStopPosition, false);
        mReadBufferStartIndex = _TotalBufferSize - _BufferStopPosition;
    }
}
