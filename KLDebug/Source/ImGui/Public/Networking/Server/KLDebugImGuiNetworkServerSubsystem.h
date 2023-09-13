#pragma once

#include "Networking/Server/KLDebugImGuiNetworkManager_Server.h"

// engine
#include "Delegates/IDelegateInstance.h"
#include "Engine/World.h"
#include "Subsystems/EngineSubsystem.h"

#include "KLDebugImGuiNetworkServerSubsystem.generated.h"

UCLASS(Transient)
class KLDEBUGIMGUI_API UKLDebugImGuiNetworkServerSubsystem final : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    //UEngineSubsystem
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    bool ShouldCreateSubsystem(UObject* _Outer) const final;
    void Deinitialize() final;
    //UEngineSubsystem

private:
    void OnWorldPostInitialize(UWorld* _World, const UWorld::InitializationValues _IVS);

private:
    FKLDebugImGuiNetworkManager_Server mServer;
    FDelegateHandle mOnWorldPostInitializeDelegateHandle;
};
