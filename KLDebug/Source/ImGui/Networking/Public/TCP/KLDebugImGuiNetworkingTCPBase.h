// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "HAL/CriticalSection.h"
#include "HAL/Platform.h"
#include "HAL/Runnable.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

class FSocket;
class UWorld;

// Basic interface to have a server running on its own thread
// based on FCookOnTheFlyServerTCP and FTcpMessageTransport and FTcpMessageTransportConnection
class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingTCPBase : public FRunnable
{
public:
    // FRunnable
    bool Init() override;
    void Stop() final;
    uint32 Run() final;
    void Exit() override;
    // FRunnable

    virtual void CreateSocket() = 0;
    UE_NODISCARD virtual bool IsValid() const;

protected:
    virtual void RunChild() = 0;
    UE_NODISCARD virtual float GetSleepTime() const;

    void ClearListenerSocket();

    UE_NODISCARD bool ShouldStop() const;

protected:
    FSocket* mListenerSocket = nullptr;
    float mSecondsToSleep = 0.2f;
    mutable FCriticalSection mGameThreadUpdateLock;

private:
    bool mStop = false;
};

inline void FKLDebugImGuiNetworkingTCPBase::Stop()
{
    mStop = true;
}

inline bool FKLDebugImGuiNetworkingTCPBase::ShouldStop() const
{
    return mStop;
}

inline bool FKLDebugImGuiNetworkingTCPBase::IsValid() const
{
    return mListenerSocket != nullptr;
}

inline float FKLDebugImGuiNetworkingTCPBase::GetSleepTime() const
{
    return mSecondsToSleep;
}