// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "TCP/KLDebugImGuiNetworkingConnectionGetPendingBuffer.h"
#include "TCP/KLDebugImGuiNetworkingPendingMessage.h"
#include "TCP/KLDebugImGuiNetworkingPendingSplittedMessage.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Misc/DateTime.h"
#include "Misc/NetworkGuid.h"
#include "Sockets.h"
#include "Templates/UnrealTemplate.h"

class FArchive;
class FKLDebugImGuiNetworkingMessage_Header;
class FSocket;
class ISocketSubsystem;

// base class representing a socket connection established with someone
class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingTCPCachedConnectionBase : public FNoncopyable
{
public:
    explicit FKLDebugImGuiNetworkingTCPCachedConnectionBase(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _Socket);
    virtual ~FKLDebugImGuiNetworkingTCPCachedConnectionBase();

    void Shutdown();
    void Tick();

    UE_NODISCARD virtual bool IsValid() const;
    UE_NODISCARD const FSocket& GetSocket() const;
    UE_NODISCARD bool HasNewReadData() const;

protected:
    // this is tick to read the data from the network buffer.
    // If no one else should read more the buffer after this is called, the _Reader should be set at the end
    //_BufferReadStopPosition represent the stop read position use to compute how much of the buffer we want to keep
    UE_NODISCARD virtual bool ReadBufferChildHasHandleMessage(const FKLDebugImGuiNetworkingMessage_Header& _Header, FArchive& _Data);
    // this is use to tick the child and see if we are in an invalid state.
    // NOTER: this is called after TickChildReadBufferGetCanReadMore
    UE_NODISCARD virtual bool TickChild() = 0;
    // allow the child to directly write in the write buffer if needed
    virtual void TickChildWriteBuffer(FArchive& _Writer);
    virtual void OnSendDataResultChild(const bool _DataSent);

    void ClearSocket(ISocketSubsystem& _SocketSubsystem);
    UE_NODISCARD FSocket& GetSocketMutable() const;

    UE_NODISCARD bool TickReadWriteBuffers(const bool _HasReadBytes, FArchive& _Reader);

protected:
    UE_NODISCARD TArray<uint8>& GetWriteBuffer();
    UE_NODISCARD FKLDebugImGuiNetworkingConnectionGetPendingBuffer GetReadBuffer();
    virtual UE_NODISCARD bool CheckConnectionStatus();

private:
    int32 TickReadConnectionData();
    UE_NODISCARD bool TickReadMessagesWithHeader(FArchive& _Reader);
    void TickWriteConnectionData();

    void UpdateBufferStartIndex(const uint32 _BufferStopPosition, const uint32 _TotalBufferSize);

    void SendData(TArray<uint8>& _Buffer);

protected:
    FDateTime mPendingConnectionTime;

private:
    FNetworkGUID mOwnerNetGUID;
    TArray<uint8> mReceiveBuffer;
    TArray<uint8> mTempMessageBuffer;
    TArray<uint8> mWriteBuffers;
    TArray<FKLDebugImGuiNetworkingPendingMessage> mPendingMessages;
    // these are messages that have been splitted server side and we are waiting to receive all packes for them
    TArray<FKLDebugImGuiNetworkingPendingSplittedMessage> mPendingSplittedMessages;
    FSocket* mSocket = nullptr;
    uint32 mReadBufferStartIndex = 0;
    uint32 mInitialReadBufferSize = 0;
    bool mHasNewReadData = false;
};

inline bool FKLDebugImGuiNetworkingTCPCachedConnectionBase::ReadBufferChildHasHandleMessage(const FKLDebugImGuiNetworkingMessage_Header& _Header, FArchive& _Data)
{
    return false;
}

inline void FKLDebugImGuiNetworkingTCPCachedConnectionBase::TickChildWriteBuffer(FArchive& _Writer)
{
}

inline void FKLDebugImGuiNetworkingTCPCachedConnectionBase::OnSendDataResultChild(const bool _DataSent)
{
}

inline bool FKLDebugImGuiNetworkingTCPCachedConnectionBase::IsValid() const
{
    return mSocket != nullptr;
}

inline FSocket& FKLDebugImGuiNetworkingTCPCachedConnectionBase::GetSocketMutable() const
{
    checkf(mSocket != nullptr, TEXT("socket must be valid"));
    return *mSocket;
}

inline const FSocket& FKLDebugImGuiNetworkingTCPCachedConnectionBase::GetSocket() const
{
    return GetSocketMutable();
}

inline TArray<uint8>& FKLDebugImGuiNetworkingTCPCachedConnectionBase::GetWriteBuffer()
{
    return mWriteBuffers;
}

inline FKLDebugImGuiNetworkingConnectionGetPendingBuffer FKLDebugImGuiNetworkingTCPCachedConnectionBase::GetReadBuffer()
{
    return FKLDebugImGuiNetworkingConnectionGetPendingBuffer{ mPendingMessages, mHasNewReadData };
}

inline bool FKLDebugImGuiNetworkingTCPCachedConnectionBase::HasNewReadData() const
{
    return mHasNewReadData;
}
