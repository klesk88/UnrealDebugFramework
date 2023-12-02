// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Common/KLDebugNetworkingManager_Base.h"

#include "Settings/KLDebugNetworkingSettings.h"

// utils
#include "Networking/Runtime/Public/Helpers/KLDebugNetworkingHelpers.h"
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Async/Async.h"
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Misc/ScopeLock.h"
#include "Serialization/BitWriter.h"
#include "Sockets.h"

void FKLDebugNetworkingManager_Base::InitFromWorld(UWorld& _World)
{
    mWorld = &_World;
    mWorldNetworkID = KL::Debug::Networking::Helpers::TryGetNetworkGuid(_World);
    InitFromWorldChild(_World);
}

void FKLDebugNetworkingManager_Base::ClearFromWorld(const UWorld& _World)
{
    if (mWorld.Get() != &_World)
    {
        return;
    }

    UnregisterTick();
    ClearFromWorldChild(_World);
    mWorld.Reset();
}

const UKLDebugNetworkingSettings& FKLDebugNetworkingManager_Base::GetNetworkConfig() const
{
    const UKLDebugNetworkingSettings& Config = UKLDebugNetworkingSettings::Get();
    return Config;
}

void FKLDebugNetworkingManager_Base::StartTick()
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugNetworkingManagerBase_StartTick);

    FScopeLock Lock(&mTickLock);

    if (mTickObject.IsTickFunctionEnabled())
    {
        return;
    }

    if (mIsPendingAsyncTick)
    {
        mShouldTick = true;
        return;
    }

    if (IsInGameThread())
    {
        mTickObject.SetTickFunctionEnable(true);
    }
    else
    {
        mIsPendingAsyncTick = true;
        AsyncTask(ENamedThreads::GameThread, [this] {
            UpdateTickStatus();
        });
    }
}

void FKLDebugNetworkingManager_Base::StopTick()
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugNetworkingManagerBase_StopTick);

    FScopeLock Lock(&mTickLock);

    if (!mTickObject.IsTickFunctionEnabled())
    {
        return;
    }

    if (mIsPendingAsyncTick)
    {
        mShouldTick = false;
        return;
    }

    if (IsInGameThread())
    {
        mTickObject.SetTickFunctionEnable(false);
    }
    else
    {
        mIsPendingAsyncTick = true;
        AsyncTask(ENamedThreads::GameThread, [this] {
            UpdateTickStatus();
        });
    }
}

void FKLDebugNetworkingManager_Base::UpdateTickStatus()
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugNetworkingManagerBase_UpdateTickStatus);

    FScopeLock Lock(&mTickLock);
    mTickObject.SetTickFunctionEnable(mShouldTick);
    mIsPendingAsyncTick = false;
}

void FKLDebugNetworkingManager_Base::UnregisterTick()
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugNetworkingManagerBase_UnregisterTick);
    FScopeLock Lock(&mTickLock);

    mTickUnregister = true;
    if (mTickObject.IsTickFunctionRegistered())
    {
        mTickObject.UnRegisterTickFunction();
    }

    mTickObject.ClearDelegate();
}

void FKLDebugNetworkingManager_Base::InitTick(UWorld& _World)
{
    mTickObject.bStartWithTickEnabled = false;
    mTickObject.bCanEverTick = true;
    mTickObject.bTickEvenWhenPaused = true;
    mTickObject.bRunOnAnyThread = true;
    mTickObject.TickInterval = 0.f;
    mTickObject.TickGroup = ETickingGroup::TG_LastDemotable;
    mTickObject.EndTickGroup = ETickingGroup::TG_LastDemotable;

    mTickObject.BindDelegate(FOnTick::CreateRaw(this, &FKLDebugNetworkingManager_Base::Tick));
    mTickObject.RegisterTickFunction(_World.PersistentLevel);
}

UWorld& FKLDebugNetworkingManager_Base::GetWorldMutable() const
{
    checkf(mWorld.IsValid(), TEXT("must be valid still"));
    return *mWorld.Get();
}
