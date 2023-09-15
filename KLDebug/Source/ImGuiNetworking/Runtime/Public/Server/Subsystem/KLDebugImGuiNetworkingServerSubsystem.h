#pragma once

#include "Server/KLDebugImGuiNetworkingManager_Server.h"

//engine
#include "Subsystems/WorldSubsystem.h"


#include "KLDebugImGuiNetworkingServerSubsystem.generated.h"

UCLASS(Transient)
class KLDEBUGIMGUINETWORKINGRUNTIME_API UKLDebugImGuiNetworkingServerSubsystem final : public UWorldSubsystem
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
    static FKLDebugImGuiNetworkingManager_Server mServer;
};
