// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "HAL/CriticalSection.h"
#include "HAL/Platform.h"
#include "HAL/Runnable.h"

// Basic interface to have a server running on its own thread
// based on FCookOnTheFlyServerTCP and FTcpMessageTransport and FTcpMessageTransportConnection
class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingTCPBase : public FRunnable
{
public:
    // FRunnable
    uint32 Run() final;
    void Stop() final;
    // FRunnable

protected:
    virtual void RunChild() = 0;
    UE_NODISCARD virtual float GetSleepTime() const;

    UE_NODISCARD bool ShouldStop() const;

protected:
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

inline float FKLDebugImGuiNetworkingTCPBase::GetSleepTime() const
{
    return mSecondsToSleep;
}