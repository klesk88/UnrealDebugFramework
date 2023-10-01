#pragma once

//engine
#include "Subsystems/WorldSubsystem.h"

#include "KLDebugImGuiServerSubsystem_World.generated.h"

class UWorld;

UCLASS(Transient)
class KLDEBUGIMGUISERVER_API UKLDebugImGuiServerSubsystem_World final : public UWorldSubsystem
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
};
