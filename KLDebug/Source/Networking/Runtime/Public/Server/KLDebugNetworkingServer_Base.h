#pragma once

//engine
#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeCounter.h"

class FSocket;

// Basic interface to have a server running on its own thread
// based on FCookOnTheFlyServerTCP and FTcpMessageTransport and FTcpMessageTransportConnection
class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingServer_Base : public FRunnable
{
public:
    //FRunnable
    void Stop() final;
    //FRunnable

protected:
    UE_NODISCARD bool ShouldStop() const;

private:
    FSocket* mSocket = nullptr;
    bool mStop = false;
};

inline void FKLDebugNetworkingServer_Base::Stop()
{
    mStop = true;
}

inline bool FKLDebugNetworkingServer_Base::ShouldStop() const
{
    return mStop;
}
