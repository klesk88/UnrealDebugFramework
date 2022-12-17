#pragma once

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"

#include "KLDebugEngineSubsystem.generated.h"

class IKLDebugWindow;

UCLASS()
class KLDEBUGRUNTIME_API UKLDebugEngineSubsystem final : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    static UKLDebugEngineSubsystem* Get();

    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;

private:
    void GatherWindows();

private:
    TArray<IKLDebugWindow*> mDebugWindows;
};
