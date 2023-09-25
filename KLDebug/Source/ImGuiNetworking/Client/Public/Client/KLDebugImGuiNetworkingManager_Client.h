#pragma once

//runtime module
#include "ImGuiNetworking/Runtime/Public/Common/KLDebugImGuiNetworkingManager_Base.h"
#include "ImGuiNetworking/Runtime/Public/Message/FeatureUpdate/KLDebugImGuiNetworkingMessage_FeatureStatusUpdate.h"

//engine
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Delegates/IDelegateInstance.h"
#include "Misc/NetworkGuid.h"
#include "Templates/SharedPointer.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FInternetAddr;
class FKLDebugImGuiFeatureStatusUpdateData;
class FNetBitWriter;
class FSocket;
class FString;
class UPackageMap;
class UWorld;

struct FBitReader;
struct FBitWriter;

class KLDEBUGIMGUINETWORKINGCLIENT_API FKLDebugImGuiNetworkingManager_Client final : public FKLDebugImGuiNetworkingManager_Base
{
public:
    //FKLDebugImGuiNetworkManager_Base
    UE_NODISCARD bool IsSocketRegistered() const;
    //FKLDebugImGuiNetworkManager_Base

private:
    //FKLDebugImGuiNetworkManager_Base
    void Tick(const float _DeltaTime) final;
    void InitChild(UWorld& _World) final;
    void ClearChild() final;
    //FKLDebugImGuiNetworkManager_Base

    void InitServerSocket(const FString& _SocketName, const FString& _IP, const int32 _Port, const int32 _ReceiveBufferSize, const int32 _SendBufferSize);

    void OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData);

    void TickReadData();
    void TickWriteData();

    void TryReconnect();

    void WritePendingFeaturesStatusUpdate(FNetBitWriter& _BitWriter);

    void ReadData(FBitReader& _Reader);

    UE_NODISCARD UPackageMap* GetServerPackageMap() const;

private:
    TArray<uint8> mReceiverDataBuffer;
    TArray<FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate> mPendingFeaturesStatusUpdates;
    TMap<FObjectKey, FNetworkGUID> mObjectToNetworkID;
    TSharedPtr<FInternetAddr> mServerAddress;
    FDelegateHandle mOnFeaturesUpdatedDelegateHandle;
    FSocket* mServerSocket = nullptr;
    int32 mSendBufferSize = 0;
    float mReconnectionTime = 1.f;
    float mLastTimeTryToConnect = 0.f;
};

inline bool FKLDebugImGuiNetworkingManager_Client::IsSocketRegistered() const
{
    return mServerSocket != nullptr;
}