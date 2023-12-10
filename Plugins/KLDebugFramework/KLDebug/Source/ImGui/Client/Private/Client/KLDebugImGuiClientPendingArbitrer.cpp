// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Client/KLDebugImGuiClientPendingArbitrer.h"

// modules
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Sockets.h"
#include "SocketSubsystem.h"

FKLDebugImGuiClientPendingArbitrer::~FKLDebugImGuiClientPendingArbitrer()
{
    Shutdown();
}

void FKLDebugImGuiClientPendingArbitrer::Init(FSocket& _Socket)
{
    mArbitrer = &_Socket;
}

void FKLDebugImGuiClientPendingArbitrer::SendData()
{
    checkf(!mDataToSend.IsEmpty(), TEXT("must have data"));
    checkf(mArbitrer != nullptr, TEXT("must be valid"));

    int32 BytesSent = 0;
    if (!mArbitrer->Send(mDataToSend.GetData(), mDataToSend.Num(), BytesSent))
    {
        return;
    }

    if (BytesSent == -1)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiClientPendingArbitrer::SendData>> Supposed to send [%d] but we didn't send anything"), mDataToSend.Num(), BytesSent);
        return;
    }

    Shutdown();
    UE_CLOG(BytesSent != mDataToSend.Num(), LogKL_Debug, Warning, TEXT("FKLDebugImGuiClientPendingArbitrer::SendData>> Supposed to send [%d] but we sent just [%d]"), mDataToSend.Num(), BytesSent);
    mDataToSend.Reset();
}

void FKLDebugImGuiClientPendingArbitrer::Shutdown()
{
    if (!mArbitrer)
    {
        return;
    }

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (SocketSubsystem)
    {
        mArbitrer->Close();
        SocketSubsystem->DestroySocket(mArbitrer);
        mArbitrer = nullptr;
    }
}
