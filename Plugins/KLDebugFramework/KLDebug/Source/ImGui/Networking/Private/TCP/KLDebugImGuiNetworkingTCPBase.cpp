// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "TCP/KLDebugImGuiNetworkingTCPBase.h"

// engine
#include "HAL/PlatformProcess.h"
#include "Misc/ScopeTryLock.h"
#include "Sockets.h"

uint32 FKLDebugImGuiNetworkingTCPBase::Run()
{
    while (!mStop)
    {
        {
            FScopeTryLock Lock(&mGameThreadUpdateLock);
            if (Lock.IsLocked())
            {
                RunChild();
            }
        }

        const float SleepTime = GetSleepTime();
        FPlatformProcess::Sleep(SleepTime);
    }

    return 0;
}

void FKLDebugImGuiNetworkingTCPBase::Exit()
{
    ClearListenerSocket();
}

void FKLDebugImGuiNetworkingTCPBase::ClearListenerSocket()
{
    if (mListenerSocket)
    {
        mListenerSocket->Close();
        mListenerSocket = nullptr;
    }
}
