#pragma once

#include "Networking/KLDebugImGuiNetworkManager_Base.h"

//engine
#include "Containers/Array.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FSocket;
class FString;
class UWorld;

struct FBitReader;
struct FBitWriter;

class KLDEBUGIMGUI_API FKLDebugImGuiNetworkManager_Client final : public FKLDebugImGuiNetworkManager_Base
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

    void InitServerSocket(const FString& _SocketName, const FString& _IP, const int32 _Port, const int32 _ReceiveBufferSize, const int32 _SendBufferSize);

    void TickReadData();
    void TickWriteData();

    void ReadData(FBitReader& _Reader);

private:
    TArray<uint8> mReceiverDataBuffer;
    TWeakObjectPtr<UWorld> mWorld;
    FSocket* mServerSocket = nullptr;
    int32 mSendBufferSize = 0;

    bool hasWritten = false;
};

inline bool FKLDebugImGuiNetworkManager_Client::IsSocketRegistered() const
{
    return mServerSocket != nullptr;
}