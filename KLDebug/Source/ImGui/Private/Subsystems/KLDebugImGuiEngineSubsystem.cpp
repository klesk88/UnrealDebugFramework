#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

#include "Config/KLDebugImGuiConfig.h"

// KLUnrealImGui module
#include "UnrealImGui/Public/KLUnrealImGuiModule.h"
// UnrealImGui module
#include "UnrealImGui/Public/ImGuiDelegates.h"
#include "UnrealImGui/Public/UnrealImGuiModule.h"

// engine
#include "Engine/Engine.h"

namespace KL::Debug::ImGui::EngineSubsystem
{
    FImGuiDelegateHandle mImGuiDelegate;
}

////////////////////////////////////////////////////////////////////

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
    InitFromConfig();

    mFeaturesContainer.Initialize();
    RegisterCallbacks();
    mInputManager.Init();
}

void UKLDebugImGuiEngineSubsystem::InitFromConfig()
{
    const UKLDebugImGuiConfig& ImGuiConfig = UKLDebugImGuiConfig::Get();

    mImGuiWindow = ImGuiConfig.GetImGuiWindow();
}

void UKLDebugImGuiEngineSubsystem::RegisterCallbacks()
{
    FKLUnrealImGuiModule& KLUnrealImgui = FKLUnrealImGuiModule::Get();
    KL::Debug::ImGui::EngineSubsystem::mImGuiDelegate = KLUnrealImgui.AddMultiContextImGuiDelegate(FImGuiDelegate::CreateUObject(this, &UKLDebugImGuiEngineSubsystem::Update));
}

void UKLDebugImGuiEngineSubsystem::Deinitialize()
{
    mInputManager.Shutdown();
    UnreagisterCallbacks();
    mFeaturesContainer.Shutdown();
}

void UKLDebugImGuiEngineSubsystem::UnreagisterCallbacks()
{
    if (FKLUnrealImGuiModule* KLUnrealImgui = FKLUnrealImGuiModule::TryGet())
    {
        if (KL::Debug::ImGui::EngineSubsystem::mImGuiDelegate.IsValid())
        {
            KLUnrealImgui->RemoveImGuiDelegate(KL::Debug::ImGui::EngineSubsystem::mImGuiDelegate);
            KL::Debug::ImGui::EngineSubsystem::mImGuiDelegate.Reset();
        }
    }
}

void UKLDebugImGuiEngineSubsystem::Update(const UWorld& _World)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiEngineSubsystem_Update);

    FKLDebugImGuiWindow& ImGuiWindow = mImGuiWindow.GetMutable<FKLDebugImGuiWindow>();
    ImGuiWindow.Update(_World);
}
