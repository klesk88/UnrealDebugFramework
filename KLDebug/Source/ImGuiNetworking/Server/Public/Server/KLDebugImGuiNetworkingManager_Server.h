#pragma once

#include "Server/KLDebugImGuiNetworkingCacheConnection.h"

//networking runtime module
#include "ImGuiNetworking/Runtime/Public/Common/KLDebugImGuiNetworkingManager_Base.h"

// engine
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Templates/RefCounting.h"
#include "UObject/NameTypes.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class FSocket;
class FString;
class UWorld;

struct FBitReader;
struct FBitWriter;

class KLDEBUGIMGUINETWORKINGSERVER_API FKLDebugImGuiNetworkingManager_Server final : public FKLDebugImGuiNetworkingManager_Base
{
private:
    enum class EReadWriteDataResult : uint8
    {
        Succeeded = 0,
        Fail
    };

public:
    //FKLDebugImGuiNetworkManager_Base
    UE_NODISCARD bool IsSocketRegistered() const;
    //FKLDebugImGuiNetworkManager_Base

    void InitFromEngine();
    void ClearFromEngine();

private:
    //FKLDebugImGuiNetworkManager_Base
    void Tick(const float _DeltaTime) final;
    void InitFromWorldChild(UWorld& _World) final;
    void ClearFromWorldChild(const UWorld& _World) final;
    //FKLDebugImGuiNetworkManager_Base

    void InitListenerSocket(const FString& _SocketName, const int32 _Port, const int32 _ReceiveBufferSize);

    void TickListenerSocket();
    void TickConnections();

    UE_NODISCARD UPackageMap* GetClientPackageMap(const UWorld& _World, const FSocket& _ClientSocket) const;

    UE_NODISCARD FKLDebugImGuiNetworkingManager_Server::EReadWriteDataResult ReceiveConnectionData(FKLDebugImGuiNetworkingCacheConnection& _Connection, FSocket& _ClientSocket);
    UE_NODISCARD EReadWriteDataResult ReadData(FKLDebugImGuiNetworkingCacheConnection& _Connection, FBitReader& _Reader);
    UE_NODISCARD EReadWriteDataResult Rcv_HandleClientFeatureStatusUpdate(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const UWorld& _World, FKLDebugImGuiNetworkingCacheConnection& _Connection, FBitReader& _Reader);

    void SendConnectionData(const UWorld& _World, const FKLDebugImGuiNetworkingCacheConnection& _Connection, FSocket& _ClientSocket) const;

private:
    TArray<TRefCountPtr<FKLDebugImGuiNetworkingCacheConnection>> mConnectedSockets;
    TArray<uint8> mReceiverDataBuffer;
    TMap<FName, uint16> mFeatureToContainerIndex;
    FSocket* mListenerSocket = nullptr;
    uint32 mClientReadBufferSize = 0;
    uint32 mClientWriteBufferSize = 0;
};

inline bool FKLDebugImGuiNetworkingManager_Server::IsSocketRegistered() const
{
    return mListenerSocket != nullptr;
}