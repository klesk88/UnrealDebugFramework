#pragma once

#include "Client/KLDebugImGuiNetworkingManager_Client.h"

//engine
#include "Subsystems/WorldSubsystem.h"

#include "KLDebugImGuiNetworkingClientSubsystem_World.generated.h"

class UWorld;

UCLASS(Transient)
class KLDEBUGIMGUINETWORKINGCLIENT_API UKLDebugImGuiNetworkingClientSubsystem_World final : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    //UWorldSubsystem
    bool ShouldCreateSubsystem(UObject* _Outer) const final;
    void Deinitialize() final;
    void OnWorldBeginPlay(UWorld& _World) final;
    //UWorldSubsystem

private:
    UE_NODISCARD bool IsValid(const UWorld& _World) const;

private:
    FKLDebugImGuiNetworkingManager_Client mClient;
};
