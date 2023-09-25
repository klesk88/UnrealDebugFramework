#include "Common/KLDebugImGuiNetworkingManager_Base.h"

#include "Settings/KLDebugImGuiNetworkingSettings.h"

//utils
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Serialization/BitWriter.h"
#include "Sockets.h"

void FKLDebugImGuiNetworkingManager_Base::InitFromWorld(UWorld& _World)
{
    mWorld = &_World;
    InitFromWorldChild(_World);
}

void FKLDebugImGuiNetworkingManager_Base::ClearFromWorld(const UWorld& _World)
{
    if (mWorld.Get() != &_World)
    {
        return;
    }

    UnregisterTick();
    ClearFromWorldChild(_World);
    mWorld.Reset();
}

const UKLDebugImGuiNetworkingSettings& FKLDebugImGuiNetworkingManager_Base::GetNetworkConfig() const
{
    const UKLDebugImGuiNetworkingSettings& Config = UKLDebugImGuiNetworkingSettings::Get();
    return Config;
}

void FKLDebugImGuiNetworkingManager_Base::UnregisterTick()
{
    if (mTickObject.IsTickFunctionRegistered())
    {
        mTickObject.UnRegisterTickFunction();
    }

    mTickObject.ClearDelegate();
}

bool FKLDebugImGuiNetworkingManager_Base::SendData(FSocket& _Socket, FBitWriter& _Writer) const
{
    int32 BytesSent = 0;
    _Socket.Send(_Writer.GetData(), _Writer.GetNumBytes(), BytesSent);

    if (BytesSent <= 0)
    {
        UE_LOG(LogKL_Debug, Log, TEXT("SendData IP address valid but no data sent"));
        return false;
    }

    return true;
}

void FKLDebugImGuiNetworkingManager_Base::InitTick(UWorld& _World)
{
    mTickObject.SetTickFunctionEnable(true);
    mTickObject.bStartWithTickEnabled = true;
    mTickObject.bCanEverTick = true;
    mTickObject.bTickEvenWhenPaused = true;
    mTickObject.bRunOnAnyThread = true;
    mTickObject.TickInterval = 0.f;
    mTickObject.TickGroup = ETickingGroup::TG_LastDemotable;
    mTickObject.EndTickGroup = ETickingGroup::TG_LastDemotable;

    mTickObject.BindDelegate(FOnTick::CreateRaw(this, &FKLDebugImGuiNetworkingManager_Base::Tick));

    mTickObject.RegisterTickFunction(_World.PersistentLevel);
}

UWorld& FKLDebugImGuiNetworkingManager_Base::GetWorldMutable() const
{
    checkf(mWorld.IsValid(), TEXT("must be valid still"));
    return *mWorld.Get();
}
