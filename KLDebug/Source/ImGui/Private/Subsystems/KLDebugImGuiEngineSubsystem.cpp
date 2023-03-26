#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

#include "Config/KLDebugImGuiConfig.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Interface/Subsystem/KLDebugImGuiFeatureInterface_EngineSubsystem.h"
#include "Subsystems/KLDebugImGuiSubsystemUpdatable.h"

// KLUnrealImGui module
#include "UnrealImGui/Public/KLUnrealImGuiModule.h"
// UnrealImGui module
#include "UnrealImGui/Public/ImGuiDelegates.h"
#include "UnrealImGui/Public/UnrealImGuiModule.h"
// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Engine/Engine.h"
#include "UObject/Object.h"

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

void UKLDebugImGuiEngineSubsystem::AddUpdatableSystem(IKLDebugImGuiSubsystemUpdatable& _System)
{
#if DO_ENSURE
    for (const TWeakInterfacePtr<IKLDebugImGuiSubsystemUpdatable>& System : mUpdatableSystems)
    {
        if (System.Get() == &_System)
        {
            ensureMsgf(false, TEXT("system already present"));
            break;
        }
    }
#endif

    mPendingUpdatableSystems.Emplace(&_System);
}

void UKLDebugImGuiEngineSubsystem::RemoveUpdatableSystem(const IKLDebugImGuiSubsystemUpdatable& _System)
{
    for (int32 i = 0; i < mUpdatableSystems.Num(); ++i)
    {
        if (mUpdatableSystems[i].Get() == &_System)
        {
            mUpdatableSystems.RemoveAtSwap(i);
            return;
        }
    }

    ensureMsgf(false, TEXT("system not found"));
}

void UKLDebugImGuiEngineSubsystem::Initialize(FSubsystemCollectionBase& _Collection)
{
    InitFromConfig();

    mFeatureContainersManger.Initialize();
    RegisterCallbacks();
    mInputManager.Init();

    FKLDebugImGuiWindow& ImGuiWindow = mImGuiWindow.GetMutable<FKLDebugImGuiWindow>();
    ImGuiWindow.Init();

    mPendingUpdatableSystems.Reserve(10);
    mUpdatableSystems.Reserve(10);
}

void UKLDebugImGuiEngineSubsystem::InitFromConfig()
{
    const UKLDebugImGuiConfig& ImGuiConfig = UKLDebugImGuiConfig::Get();

    mImGuiWindow = ImGuiConfig.GetImGuiWindow();
}

void UKLDebugImGuiEngineSubsystem::RegisterCallbacks()
{
    FKLUnrealImGuiModule& KLUnrealImgui               = FKLUnrealImGuiModule::Get();
    KL::Debug::ImGui::EngineSubsystem::mImGuiDelegate = KLUnrealImgui.AddMultiContextImGuiDelegate(FImGuiDelegate::CreateUObject(this, &UKLDebugImGuiEngineSubsystem::Update));
}

void UKLDebugImGuiEngineSubsystem::Deinitialize()
{
    FKLDebugImGuiWindow& ImGuiWindow = mImGuiWindow.GetMutable<FKLDebugImGuiWindow>();
    ImGuiWindow.Shutdown();

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

    AddPendingUpdatableSystems();

    FKLDebugImGuiWindow& ImGuiWindow = mImGuiWindow.GetMutable<FKLDebugImGuiWindow>();
    ImGuiWindow.Update(_World);

    UpdateSystems(_World);
}

void UKLDebugImGuiEngineSubsystem::AddPendingUpdatableSystems()
{
    for (const TWeakInterfacePtr<IKLDebugImGuiSubsystemUpdatable>& UpdatableSystem : mPendingUpdatableSystems)
    {
        if (!UpdatableSystem.IsValid())
        {
            continue;
        }

        UpdatableSystem->Initialize(mFeatureContainersManger);
        mUpdatableSystems.Emplace(UpdatableSystem);
    }

    mPendingUpdatableSystems.Reset();
}

void UKLDebugImGuiEngineSubsystem::UpdateSystems(const UWorld& _World)
{
    for (const TWeakInterfacePtr<IKLDebugImGuiSubsystemUpdatable>& UpdatableSystem : mUpdatableSystems)
    {
        if (!UpdatableSystem.IsValid())
        {
            continue;
        }

        UpdatableSystem->Update(_World, mFeatureContainersManger);
    }
}

void UKLDebugImGuiEngineSubsystem::DrawImGui(const UWorld& _World)
{
    const ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings;
    if (!ImGui::Begin("DebugEditor", nullptr, WindowFlags))
    {
        return;
    }

    if (ImGui::BeginTabBar("Engine", ImGuiTabBarFlags_::ImGuiTabBarFlags_None))
    {
        FKLDebugImGuiFeatureContainerBase& EngineContainer = mFeatureContainersManger.GetContainerMutable(EContainerType::ENGINE_SUBSYTSTEM);
        FKLDebugImGuiFeaturesIterator      Iterator        = EngineContainer.GetFeaturesIterator();
        for (; Iterator; ++Iterator)
        {
            IKLDebugImGuiFeatureInterface_EngineSubsystem& EngineFeatureInterface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_EngineSubsystem>();
            EngineFeatureInterface.DrawImGui(_World);
        }

        ImGui::EndTabBar();
    }

    for (const TWeakInterfacePtr<IKLDebugImGuiSubsystemUpdatable>& UpdatableSystem : mUpdatableSystems)
    {
        if (!UpdatableSystem.IsValid())
        {
            continue;
        }

        UpdatableSystem->Update(_World, mFeatureContainersManger);
    }

    ImGui::End();
}
