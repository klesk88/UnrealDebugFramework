#include "Subsystems/KLDebugEngineSubsystem.h"

#include "Window/Interface/KLDebugWindowInterface.h"
#include "Window/Manager/KLDebugWindowManager.h"
#include "Window/Manager/KLDebugWindowManagerEntryBase.h"

// engine
#include "Engine/Engine.h"

UKLDebugEngineSubsystem* UKLDebugEngineSubsystem::Get()
{
    if (GEngine)
    {
        return GEngine->GetEngineSubsystem<UKLDebugEngineSubsystem>();
    }

    return nullptr;
}

void UKLDebugEngineSubsystem::Initialize(FSubsystemCollectionBase& _Collection)
{
    GatherWindows();
}

void UKLDebugEngineSubsystem::Deinitialize()
{
    mDebugWindows.Empty();
}

void UKLDebugEngineSubsystem::GatherWindows()
{
    const FKLDebugWindowManager& WindowManager = FKLDebugWindowManager::Get();
    mDebugWindows.Reserve(WindowManager.GetEntryCount());

    FKLDebugWindowManagerEntryBase* Entry = WindowManager.GetStartEntry();

    while (Entry)
    {
        IKLDebugWindow& DebugWindow = Entry->GetInterface();
        DebugWindow.Initialize();
        mDebugWindows.Emplace(&DebugWindow);
        Entry = Entry->GetNextEntry();
    }
}
