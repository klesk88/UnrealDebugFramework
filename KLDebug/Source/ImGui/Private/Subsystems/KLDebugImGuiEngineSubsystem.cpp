#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

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
    mFeaturesContainer.Initialize();
    RegisterCallbacks();
}

void UKLDebugImGuiEngineSubsystem::RegisterCallbacks()
{
    FKLUnrealImGuiModule& KLUnrealImgui = FKLUnrealImGuiModule::Get();
    KL::Debug::ImGui::EngineSubsystem::mImGuiDelegate = KLUnrealImgui.AddMultiContextImGuiDelegate(FImGuiDelegate::CreateUObject(this, &UKLDebugImGuiEngineSubsystem::Update));
}

void UKLDebugImGuiEngineSubsystem::Deinitialize()
{
    mFeaturesContainer.Shutdown();
    UnreagisterCallbacks();
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

void UKLDebugImGuiEngineSubsystem::Update()
{
}
