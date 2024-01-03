// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Server/CachedConnection/KLDebugNetworkingPendingMessage.h"
#include "Server/CachedConnection/KLDebugNetworkingPendingSplittedMessage.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "HAL/PlatformTime.h"
#include "Misc/NetworkGuid.h"
#include "Sockets.h"
#include "Templates/UnrealTemplate.h"

class FArchive;
class FKLDebugNetworkingMessage_Header;
class FKLDebugNetworkingPendingMessage;
class FSocket;
class ISocketSubsystem;

// base class representing a socket connection established with someone
class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingCachedConnectionBase : public FNoncopyable
{
public:
    explicit FKLDebugNetworkingCachedConnectionBase(const int32 _ReadBufferSize, const int32 _WriteBufferSize, FSocket& _Socket);
    virtual ~FKLDebugNetworkingCachedConnectionBase();

    UE_NODISCARD virtual bool RequiresGameThreadTick() const = 0;

    void Shutdown();
    void Tick();

    UE_NODISCARD virtual bool IsValid() const;
    UE_NODISCARD const FSocket& GetSocket() const;

protected:
    // this needs to override by the child classes to handle incoming messages
    virtual void Parallel_HandlePendingMessageChild(FKLDebugNetworkingPendingMessage&& _PendingMessage) = 0;
    // this is use to tick the child and see if we are in an invalid state.
    // NOTER: this is called after TickChildReadBufferGetCanReadMore
    UE_NODISCARD virtual bool TickChild();
    // allow the child to directly write in the write buffer if needed
    virtual void TickChildWriteBuffer(FArchive& _Writer);
    virtual void OnSendDataResultChild(const bool _DataSent);
    virtual UE_NODISCARD bool CheckConnectionStatus();

    void ClearSocket(ISocketSubsystem& _SocketSubsystem);
    UE_NODISCARD FSocket& GetSocketMutable() const;

    UE_NODISCARD bool TickReadWriteBuffers(const bool _HasReadBytes, FArchive& _Reader);
    UE_NODISCARD TArray<uint8>& GetWriteBuffer();
    UE_NODISCARD bool TickReadMessagesWithHeader(FArchive& _Reader);

private:
    int32 TickReadConnectionData();
    void TickWriteConnectionData();

    UE_NODISCARD bool HandleInternalMessages(const FKLDebugNetworkingMessage_Header& _Header, FArchive& _Data) const;

    void UpdateBufferStartIndex(const uint32 _BufferStopPosition, const uint32 _TotalBufferSize);

    void SendData(TArray<uint8>& _Buffer);

protected:
    double mPendingConnectionTime = -1.0;
    TArray<uint8> mTempMessageBuffer;

private:
    FNetworkGUID mOwnerNetGUID;
    TArray<uint8> mReceiveBuffer;
    TArray<uint8> mWriteBuffers;
    TArray<uint8> mCheckIfAliveMessage;

    // these are messages that have been splitted server side and we are waiting to receive all packes for them
    TArray<FKLDebugNetworkingPendingSplittedMessage> mPendingSplittedMessages;
    double mLastTimeConnectionUsed = -1.0;
    double mStartTimeFailToSendMessage = -1.0;
    FSocket* mSocket = nullptr;
    uint32 mReadBufferStartIndex = 0;
    uint32 mInitialReadBufferSize = 0;
};

inline bool FKLDebugNetworkingCachedConnectionBase::TickChild()
{
    return false;
}

inline void FKLDebugNetworkingCachedConnectionBase::TickChildWriteBuffer(FArchive& _Writer)
{
}

inline void FKLDebugNetworkingCachedConnectionBase::OnSendDataResultChild(const bool _DataSent)
{
}

inline bool FKLDebugNetworkingCachedConnectionBase::IsValid() const
{
    return mSocket != nullptr;
}

inline FSocket& FKLDebugNetworkingCachedConnectionBase::GetSocketMutable() const
{
    checkf(mSocket != nullptr, TEXT("socket must be valid"));
    return *mSocket;
}

inline const FSocket& FKLDebugNetworkingCachedConnectionBase::GetSocket() const
{
    return GetSocketMutable();
}

inline TArray<uint8>& FKLDebugNetworkingCachedConnectionBase::GetWriteBuffer()
{
    return mWriteBuffers;
}
