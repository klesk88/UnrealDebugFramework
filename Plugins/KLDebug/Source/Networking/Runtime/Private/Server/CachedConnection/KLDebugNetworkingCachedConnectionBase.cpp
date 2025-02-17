// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/CachedConnection/KLDebugNetworkingCachedConnectionBase.h"

#include "Log/KLDebugNetworkingLog.h"
#include "Message/Common/KLDebugNetworkingMessage_CheckIfAlive.h"
#include "Message/Header/KLDebugNetworkingMessage_Header.h"
#include "Message/Helpers/KLDebugNetworkingMessageHelpers.h"

// modules
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageDefinitions.h"

// engine
#include "Containers/ArrayView.h"
#include "HAL/Platform.h"
#include "HAL/UnrealMemory.h"
#include "Math/UnrealMathUtility.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "SocketSubsystem.h"
#include "Stats/Stats.h"
#include "Stats/Stats2.h"

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
    int32 TotalBytesRead = 0;
    uint32 StartBufferIndex = mReadBufferStartIndex;

    if (mReadBufferStartIndex == 0 && mReceiveBuffer.Num() > static_cast<int32>(mInitialReadBufferSize))
    {
        // resize the array to its original size so we dont waste too much space
        mReceiveBuffer.SetNum(mInitialReadBufferSize, EAllowShrinking::Yes);
    }

    while (Socket.HasPendingData(Size) && Size > 0)
    {
        checkf(mReceiveBuffer.IsValidIndex(StartBufferIndex), TEXT("must be valid"));
        const int32 SpaceLeftInBuffer = mReceiveBuffer.Max() - StartBufferIndex;
        if (Size > static_cast<uint32>(SpaceLeftInBuffer))
        {
            ensureMsgf(false, TEXT("resizing array should not happen"));
            const int32 NewSize = Size - SpaceLeftInBuffer + 1;
            mReceiveBuffer.SetNum(NewSize, EAllowShrinking::No);
        }

        const uint32 MaxReadSize = FMath::Min(Size, static_cast<uint32>(mReceiveBuffer.Num() - StartBufferIndex));
        Socket.Recv(&mReceiveBuffer[StartBufferIndex], MaxReadSize, BytesRead);
        Size = 0;
        if (BytesRead == 0)
        {
            UE_LOG(LogKLDebug_Networking, Warning, TEXT("FKLDebugNetworkingCachedConnectionBase::TickReadConnectionData>> Socket had pending data but didnt read any bytes"));
            continue;
        }

        StartBufferIndex += BytesRead;
        TotalBytesRead += BytesRead;
        ensureMsgf(StartBufferIndex < static_cast<uint32>(mReceiveBuffer.Num()), TEXT("StartBufferIndex must always be les then the buffer size"));
    }

    if (TotalBytesRead != 0)
    {
        mLastTimeConnectionUsed = FPlatformTime::Seconds();
    }

    return TotalBytesRead;
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

    auto ReadMessagesLambda = [this](const FKLDebugNetworkingMessage_Header& _MessageHeader, FArchive& _MessageData) -> void {
        if (HandleInternalMessages(_MessageHeader, _MessageData))
        {
            return;
        }

        if (!_MessageHeader.IsSplittedMessage())
        {
            FKLDebugNetworkingPendingMessage NewPendingMessage{ _MessageHeader, mTempMessageBuffer };
            if (NewPendingMessage.HasData())
            {
                Parallel_HandlePendingMessageChild(MoveTemp(NewPendingMessage));
            }

            return;
        }

        const int32 Index = mPendingSplittedMessages.IndexOfByKey(_MessageHeader.GetMessageID());
        if (Index == INDEX_NONE)
        {
            mPendingSplittedMessages.Emplace(_MessageHeader, mTempMessageBuffer);
            return;
        }

        FKLDebugNetworkingPendingSplittedMessage& PendingSplitMessage = mPendingSplittedMessages[Index];
        PendingSplitMessage.AddData(mTempMessageBuffer, _MessageHeader.GetMessageDataOffset());
        if (!PendingSplitMessage.IsFullyReceived())
        {
            return;
        }

        if (!PendingSplitMessage.HasFailedToReadData())
        {
            FKLDebugNetworkingPendingMessage NewPendingMessage{ PendingSplitMessage.GetHeaderMessage(), MoveTemp(PendingSplitMessage.GetDataMutable()) };

            if (NewPendingMessage.HasData())
            {
                Parallel_HandlePendingMessageChild(MoveTemp(NewPendingMessage));
            }
        }

        mPendingSplittedMessages.RemoveAtSwap(Index, 1, EAllowShrinking::No);
    };

    const uint32 StopReadLocation = KL::Debug::Networking::Message::ReadBufferGetStopReadLocation(ReadMessagesLambda, mTempMessageBuffer, _Reader);

    const int64 TotalSize = _Reader.TotalSize();
    checkf(TotalSize < TNumericLimits<uint32>::Max(), TEXT("total size too high not expected"));
    UpdateBufferStartIndex(StopReadLocation, TotalSize);

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

    switch (static_cast<EKLDebugNetworkingMessage>(_Header.GetMessageType()))
    {
    case EKLDebugNetworkingMessage::IsConnectionAlive:
    {
        return true;
    }
    default:
        return false;
    }
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
    _Buffer.RemoveAt(0, BytesSent, EAllowShrinking::No);
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
        // we read part of the buffer and we stopped at _BufferStopPosition. So free up all previous bytes but keep
        // the ones we still need to process
        mReadBufferStartIndex = _TotalBufferSize - _BufferStopPosition;
        checkf(static_cast<uint32>(mReceiveBuffer.Num()) > mReadBufferStartIndex, TEXT("out of bounds"));
        FMemory::Memcpy(mReceiveBuffer.GetData(), &mReceiveBuffer[_BufferStopPosition], static_cast<SIZE_T>(mReadBufferStartIndex));
        mReadBufferStartIndex = _TotalBufferSize - _BufferStopPosition;
    }
}
