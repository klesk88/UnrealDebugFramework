// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "IPAddress.h"
#include "Templates/SharedPointer.h"
#include "Templates/UnrealTemplate.h"

class FArchive;
class FSocket;
class ISocketSubsystem;
class UKLDebugNetworkingArbitrerSettings;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerArbitrerManager final : public FNoncopyable
{
private:
    enum class EServerArbitrerState : uint8
    {
        PendingConnection = 0,
        PendingAnswer,
        Connected,
        Failed
    };

public:
    void Init();
    void Shutdown();

    void Parallel_Tick();

    [[nodiscard]] bool HasBeenInitialized() const;

    void ArbitrerAddWorldConnection(const int32 _ServerPort, const int32 _DebugPort);
    void ArbitrerRemovedWorldConnection(const int32 _ServerPort);

private:
    void Clear();

    void CreateListenerSocket(const UKLDebugNetworkingArbitrerSettings& _ArbitrerSettings, ISocketSubsystem& _SocketSubsytem);
    void CreateSenderSocket(const UKLDebugNetworkingArbitrerSettings& _ArbitrerSettings, ISocketSubsystem& _SocketSubsytem);

    void HandlePendingConnection();
    void HandlePendingAnswer();
    void HandleConnected();

    void ReadArbitrerData();
    void SendMessage(TArray<uint8>& _SendeBuffer);

private:
    TSharedPtr<FInternetAddr> mArbitrerAddress;
    TArray<uint8> mArbitrerTempMessageBuffer;
    TArray<uint8> mArbitrerTempBuffer;
    double mTimer = 0.0;
    double mTotalTimer = 0.0;
    FSocket* mArbitrerSenderSocket = nullptr;
    FSocket* mArbitrerReceiveSocket = nullptr;
    uint32 mArbitrerAnswerPort = 0;
    bool mHasBeenInitialized = false;
    EServerArbitrerState mState = EServerArbitrerState::PendingConnection;
};

inline void FKLDebugImGuiServerArbitrerManager::HandleConnected()
{
    SendMessage(mArbitrerTempBuffer);
}

inline bool FKLDebugImGuiServerArbitrerManager::HasBeenInitialized() const
{
    return mHasBeenInitialized;
}