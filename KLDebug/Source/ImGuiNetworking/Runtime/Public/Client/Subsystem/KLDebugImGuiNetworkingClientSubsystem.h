#pragma once

#include "Client/KLDebugImGuiNetworkingManager_Client.h"

//engine
#include "Subsystems/WorldSubsystem.h"

#include "KLDebugImGuiNetworkingClientSubsystem.generated.h"

UCLASS(Transient)
class KLDEBUGIMGUINETWORKINGRUNTIME_API UKLDebugImGuiNetworkingClientSubsystem final : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    //UWorldSubsystem
    bool ShouldCreateSubsystem(UObject* _Outer) const final;
    void Deinitialize() final;
    void OnWorldBeginPlay(UWorld& _World) final;
    //UWorldSubsystem

private:
    UE_NODISCARD bool IsValidWorld(const UWorld& _World) const;

private:
    FKLDebugImGuiNetworkingManager_Client mClient;
};
