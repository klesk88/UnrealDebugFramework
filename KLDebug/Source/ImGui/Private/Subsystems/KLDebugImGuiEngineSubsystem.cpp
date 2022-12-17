#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

// engine
#include "Engine/Engine.h"

UKLDebugImGuiEngineSubsystem* UKLDebugImGuiEngineSubsystem::Get()
{
    if (GEngine)
    {
        return GEngine->GetEngineSubsystem<UKLDebugImGuiEngineSubsystem>();
    }

    return nullptr;
}

void UKLDebugImGuiEngineSubsystem::Initialize(FSubsystemCollectionBase& _Collection)
{
    mFeaturesContainer.Initialize();
}

void UKLDebugImGuiEngineSubsystem::Deinitialize()
{
    mFeaturesContainer.Shutdown();
}
