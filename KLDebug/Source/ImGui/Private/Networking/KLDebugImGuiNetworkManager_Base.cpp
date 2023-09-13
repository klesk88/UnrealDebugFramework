#include "Networking/KLDebugImGuiNetworkManager_Base.h"

#include "Config/KLDebugImGuiConfig.h"

//utils
#include "Utils/Public/KLDebugLog.h"

//engine
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Serialization/BitWriter.h"
#include "Sockets.h"

void FKLDebugImGuiNetworkManager_Base::Clear()
{
    UnregisterTick();
    ClearChild();
}

const FKLDebugImGuiConfig_Networking& FKLDebugImGuiNetworkManager_Base::GetNetworkConfig() const
{
    const UKLDebugImGuiConfig& Config = UKLDebugImGuiConfig::Get();
    return Config.GetNetworkingConfig();
}

void FKLDebugImGuiNetworkManager_Base::UnregisterTick()
{
    if (mTickObject.IsTickFunctionRegistered())
    {
        mTickObject.UnRegisterTickFunction();
    }

    mTickObject.ClearDelegate();
}

bool FKLDebugImGuiNetworkManager_Base::SendData(FSocket& _Socket, FBitWriter& _Writer) const
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

void FKLDebugImGuiNetworkManager_Base::InitTick(UWorld& _World)
{
    mTickObject.SetTickFunctionEnable(true);
    mTickObject.bStartWithTickEnabled = true;
    mTickObject.bCanEverTick = true;
    mTickObject.bTickEvenWhenPaused = true;
    mTickObject.bRunOnAnyThread = true;
    mTickObject.TickInterval = 0.f;
    mTickObject.TickGroup = ETickingGroup::TG_LastDemotable;
    mTickObject.EndTickGroup = ETickingGroup::TG_LastDemotable;

    mTickObject.BindDelegate(FOnTick::CreateRaw(this, &FKLDebugImGuiNetworkManager_Base::Tick));

    mTickObject.RegisterTickFunction(_World.PersistentLevel);
}
