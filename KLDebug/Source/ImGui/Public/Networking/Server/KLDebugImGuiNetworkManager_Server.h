#pragma once

#include "Networking/KLDebugImGuiNetworkCacheConnection.h"
#include "Networking/KLDebugImGuiNetworkManager_Base.h"

// engine
#include "Containers/Array.h"
#include "Templates/RefCounting.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FSocket;
class FString;
class UWorld;

struct FBitReader;
struct FBitWriter;

class KLDEBUGIMGUI_API FKLDebugImGuiNetworkManager_Server final : public FKLDebugImGuiNetworkManager_Base
{
public:
    //FKLDebugImGuiNetworkManager_Base
    void Init(UWorld& _World) final;
    UE_NODISCARD bool IsSocketRegistered() const;
    //FKLDebugImGuiNetworkManager_Base

private:
    //FKLDebugImGuiNetworkManager_Base
    void Tick(const float _DeltaTime) final;
    void ClearChild() final;
    //FKLDebugImGuiNetworkManager_Base

    void InitListenerSocket(const FString& _SocketName, const int32 _Port, const int32 _ReceiveBufferSize);

    void TickListenerSocket();
    void TickConnections();

    void ReceiveConnectionData(FSocket& _ClientSocket);
    void SendConnectionData(FSocket& _ClientSocket) const;

    void ReadData(FBitReader& _Reader);

private:
    TArray<TRefCountPtr<FKLDebugImGuiNetworkCacheConnection>> mConnectedSockets;
    TArray<uint8> mReceiverDataBuffer;
    TWeakObjectPtr<UWorld> mWorld;
    FSocket* mListenerSocket = nullptr;
    uint32 mClientReadBufferSize = 0;
    uint32 mClientWriteBufferSize = 0;
};

inline bool FKLDebugImGuiNetworkManager_Server::IsSocketRegistered() const
{
    return mListenerSocket != nullptr;
}