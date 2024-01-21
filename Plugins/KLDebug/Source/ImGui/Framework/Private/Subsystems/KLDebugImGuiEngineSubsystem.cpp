// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

#include "Config/KLDebugImGuiConfig.h"
#include "Subsystems/KLDebugImGuiWorldSubsystem.h"

// modules
#include "User/ThirdParty/ImGui/Public/Library/imgui.h"
#include "ThirdParty/UnrealImGui/Public/ImGuiDelegates.h"
#include "ThirdParty/UnrealImGui/Public/UnrealImGuiModule.h"
#include "UnrealImGui/Public/KLUnrealImGuiModule.h"

// engine
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstance.h"
#include "Stats/Stats2.h"
#include "UObject/Object.h"

namespace KL::Debug::ImGui::EngineSubsystem
{
    FImGuiDelegateHandle mImGuiDelegate;
}

////////////////////////////////////////////////////////////////////

UKLDebugImGuiEngineSubsystem* UKLDebugImGuiEngineSubsystem::GetMutable()
{
    if (GEngine)
    {
        return GEngine->GetEngineSubsystem<UKLDebugImGuiEngineSubsystem>();
    }

    return nullptr;
}

UMaterialInterface* UKLDebugImGuiEngineSubsystem::GetOverlayMaterial() const
{
    return OverlayMaterial.Get();
}

void UKLDebugImGuiEngineSubsystem::Initialize(FSubsystemCollectionBase& _Collection)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiEngineSubsystem_Initialize);

    InitFromConfig();

    mFeatureContainersManger.Initialize();
    RegisterCallbacks();
    mInputManager.Init();
    mBottomBarManager.Initialize();
    mModeManager.Initialize();
}

void UKLDebugImGuiEngineSubsystem::InitFromConfig()
{
    const UKLDebugImGuiConfig& ImGuiConfig = UKLDebugImGuiConfig::Get();

    OverlayMaterial = ImGuiConfig.GeOverlayMaterial().LoadSynchronous();
}

void UKLDebugImGuiEngineSubsystem::RegisterCallbacks()
{
    FKLUnrealImGuiModule& KLUnrealImgui = FKLUnrealImGuiModule::Get();
    KL::Debug::ImGui::EngineSubsystem::mImGuiDelegate = KLUnrealImgui.AddMultiContextImGuiDelegate(FImGuiDelegate::CreateUObject(this, &UKLDebugImGuiEngineSubsystem::Update));
}

void UKLDebugImGuiEngineSubsystem::Deinitialize()
{
    mModeManager.Shutdown();
    mBottomBarManager.Shutdown();
    mInputManager.Shutdown();
    UnreagisterCallbacks();
    mFeatureContainersManger.Shutdown();
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

    if (UKismetSystemLibrary::IsDedicatedServer(&_World))
    {
        return;
    }

    UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = _World.GetSubsystem<UKLDebugImGuiWorldSubsystem>();
    if (!ImGuiWorldSubsystem || !ImGuiWorldSubsystem->GetShouldTick())
    {
        return;
    }

    ImGuiWorldSubsystem->Tick(_World, mFeatureContainersManger, mBottomBarManager, mModeManager);
    ImGuiWorldSubsystem->DrawImGui(_World, mFeatureContainersManger, mBottomBarManager, mModeManager);
    ImGuiWorldSubsystem->Render(_World, mFeatureContainersManger);
}
