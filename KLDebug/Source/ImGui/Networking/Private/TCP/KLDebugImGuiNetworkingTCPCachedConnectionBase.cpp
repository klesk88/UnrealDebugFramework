#include "TCP/KLDebugImGuiNetworkingTCPCachedConnectionBase.h"

#include "Message/Header/KLDebugImGuiNetworkingMessage_Header.h"

//modules
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Containers/ArrayView.h"
#include "Math/UnrealMathUtility.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "SocketSubsystem.h"

#if DO_ENSURE
//engine
#include "Math/NumericLimits.h"
#endif

namespace KL::Debug::ImGui::Networking
{
    static int32 HeaderSize = 0;

    void InitHeaderSize()
    {
        if (HeaderSize == 0)
        {
            TArray<uint8> TempArray;
            TempArray.Reserve(30);
            FMemoryWriter Writer(TempArray);
            FKLDebugImGuiNetworkingMessage_Header DummyHeaderMessage(0, 0, 0, 0, false, 0, 0, 0, 0);
            DummyHeaderMessage.Serialize(Writer);
            HeaderSize = TempArray.Num();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////

FKLDebugImGuiNetworkingTCPCachedConnectionBase::FKLDebugImGuiNetworkingTCPCachedConnectionBase(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _Socket)
    : mSocket(&_Socket)
{
    checkf(_ReadBufferSize > 0 && _WriteBufferSize > 0, TEXT("buffers must be bigger then 0"));
    mInitialReadBufferSize = static_cast<uint32>(_ReadBufferSize);

    mWriteBuffers.Reserve(_WriteBufferSize);
    mReceiveBuffer.Init(0, _ReadBufferSize);
    mTempMessageBuffer.Reserve(500);

    KL::Debug::ImGui::Networking::InitHeaderSize();
}

FKLDebugImGuiNetworkingTCPCachedConnectionBase::~FKLDebugImGuiNetworkingTCPCachedConnectionBase()
{
    Shutdown();
}

void FKLDebugImGuiNetworkingTCPCachedConnectionBase::Shutdown()
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    check(SocketSubsystem);
    ClearSocket(*SocketSubsystem);
}

void FKLDebugImGuiNetworkingTCPCachedConnectionBase::ClearSocket(ISocketSubsystem& _SocketSubsystem)
{
    if (mSocket)
    {
        mSocket->Close();
        _SocketSubsystem.DestroySocket(mSocket);
        mSocket = nullptr;
    }
}

void FKLDebugImGuiNetworkingTCPCachedConnectionBase::UpdateSocket(ISocketSubsystem& _SocketSubsystem, FSocket& _Socket)
{
    ClearSocket(_SocketSubsystem);
    mSocket = &_Socket;
}

void FKLDebugImGuiNetworkingTCPCachedConnectionBase::Tick()
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

    ShouldShutdown = TickChild();
    if (ShouldShutdown)
    {
        Shutdown();
    }
}

bool FKLDebugImGuiNetworkingTCPCachedConnectionBase::CheckConnectionStatus()
{
    FSocket& Socket = GetSocketMutable();
    switch (Socket.GetConnectionState())
    {
    case ESocketConnectionState::SCS_Connected:
        mPendingConnectionTime = FDateTime::Now();
        break;
    case ESocketConnectionState::SCS_NotConnected:
        if (mPendingConnectionTime == FDateTime())
        {
            mPendingConnectionTime = FDateTime::Now();
        }
        else
        {
            const FTimespan TimeSpan = mPendingConnectionTime - FDateTime::Now();
            if (TimeSpan.GetTotalSeconds() > 4.f)
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

int32 FKLDebugImGuiNetworkingTCPCachedConnectionBase::TickReadConnectionData()
{
    FSocket& Socket = GetSocketMutable();
    uint32 Size = 0;
    int32 BytesRead = 0;

    if (mReadBufferStartIndex == 0 && mReceiveBuffer.Num() > static_cast<int32>(mInitialReadBufferSize))
    {
        //resize the array to its original size so we dont waste too much space
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
            UE_LOG(LogKL_Debug, Warning, TEXT("FKLDebugImGuiNetworkingTCPCachedConnectionBase::TickReadConnectionData>> Socket had pending data but didnt read any bytes"));
            continue;
        }
    }

    ensureMsgf(mReceiveBuffer.Max() == mReceiveBuffer.Num(), TEXT("num must point to the maximum value"));
    return BytesRead;
}

bool FKLDebugImGuiNetworkingTCPCachedConnectionBase::TickReadWriteBuffers(const bool _HasReadBytes, FArchive& _Reader)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPCachedConnectionBase_TickReadWriteBuffers);

    const bool ShouldShutdown = _HasReadBytes ? TickReadMessagesWithHeader(_Reader) : false;
    if (ShouldShutdown)
    {
        return true;
    }

    TickWriteConnectionData();
    return false;
}

bool FKLDebugImGuiNetworkingTCPCachedConnectionBase::TickReadMessagesWithHeader(FArchive& _Reader)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPCachedConnectionBase_TickReadMessagesWithHeader);

    if (_Reader.AtEnd())
    {
        return false;
    }

    TArray<FKLDebugImGuiNetworkingPendingMessage>& ReadBuffer = mPendingMessages;

    const int64 TotalSize = _Reader.TotalSize();
    int64 CurrentPosition = _Reader.Tell();

    while (!_Reader.AtEnd())
    {
        if (TotalSize < KL::Debug::ImGui::Networking::HeaderSize)
        {
            //not enough data for the header
            break;
        }

        const int64 HeaderCurrentPosition = _Reader.Tell();
        const FKLDebugImGuiNetworkingMessage_Header HeaderMessage{ _Reader };
        if (!HeaderMessage.IsValid())
        {
            //garbage in the stream skip one byte
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

        mTempMessageBuffer.SetNum(static_cast<int32>(HeaderMessage.GetMessageDataSize()));
        _Reader.Serialize(mTempMessageBuffer.GetData(), HeaderMessage.GetMessageDataSize());
        CurrentPosition = _Reader.Tell();
        FMemoryReader ChildClassMemoryReader(mTempMessageBuffer);
        if (ReadBufferChildHasHandleMessage(HeaderMessage, ChildClassMemoryReader))
        {
            //this message was intended for the child class
            continue;
        }

        if (!HeaderMessage.IsSplittedMessage())
        {
            FKLDebugImGuiNetworkingPendingMessage& LastMessage = ReadBuffer.Emplace_GetRef(HeaderMessage, mTempMessageBuffer);
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

        FKLDebugImGuiNetworkingPendingSplittedMessage& PendingMessage = mPendingSplittedMessages[Index];
        PendingMessage.AddData(mTempMessageBuffer, HeaderMessage.GetMessageDataOffset());
        if (!PendingMessage.IsFullyReceived())
        {
            continue;
        }

        if (!PendingMessage.HasFailedToReadData())
        {
            FKLDebugImGuiNetworkingPendingMessage& LastMessage = ReadBuffer.Emplace_GetRef(PendingMessage.GetHeaderMessage(), MoveTemp(PendingMessage.GetDataMutable()));
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

void FKLDebugImGuiNetworkingTCPCachedConnectionBase::TickWriteConnectionData()
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingTCPCachedConnectionBase_TickWriteConnectionData);

    TArray<uint8>& WriteBuffer = GetWriteBuffer();
    if (WriteBuffer.Num() != WriteBuffer.Max())
    {
        FMemoryWriter Writer(WriteBuffer, false, true);
        TickChildWriteBuffer(Writer);
    }

    if (WriteBuffer.IsEmpty())
    {
        return;
    }

    SendData(WriteBuffer);
}

void FKLDebugImGuiNetworkingTCPCachedConnectionBase::SendData(TArray<uint8>& _Buffer)
{
    int32 BytesSent = 0;
    FSocket& Socket = GetSocketMutable();
    Socket.Send(_Buffer.GetData(), _Buffer.Num(), BytesSent);
    
    const bool DataSentFully = BytesSent == _Buffer.Num();
    OnSendDataResultChild(DataSentFully);
    if (DataSentFully)
    {
        _Buffer.Reset();
        return;
    }

    UE_LOG(LogKL_Debug, Warning, TEXT("FKLDebugImGuiNetworkingTCPCachedConnectionBase::SendData>> Supposed to send [%d] but we sent just [%d]"),
        _Buffer.Num(),
        BytesSent);

    //remove the byts we managed to send
    _Buffer.RemoveAt(0, BytesSent, false);
}


void FKLDebugImGuiNetworkingTCPCachedConnectionBase::UpdateBufferStartIndex(const uint32 _BufferStopPosition, const uint32 _TotalBufferSize)
{
    if (_BufferStopPosition == 0)
    {
        //nothing read so all the buffer needs to be kept
        mReadBufferStartIndex = _TotalBufferSize;
    }
    else if (_BufferStopPosition >= _TotalBufferSize)
    {
        //we read all the buffer so we can read from position 0 next time and fill it all up again
        mReadBufferStartIndex = 0;
    }
    else
    {
        //we read part of the buffer and we stopped at Curren_BufferStopPositiontPosition. So free up all previous bytes but keep
        //the ones we still need to process
        mReceiveBuffer.RemoveAt(0, _BufferStopPosition, false);
        mReadBufferStartIndex = _TotalBufferSize - _BufferStopPosition;
    }
}
