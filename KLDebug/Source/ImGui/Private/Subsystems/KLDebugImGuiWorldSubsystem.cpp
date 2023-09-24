#include "Subsystems/KLDebugImGuiWorldSubsystem.h"

#include "Config/KLDebugImGuiConfig.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"
#include "Feature/Interface/Subsystem/KLDebugImGuiFeatureInterface_ObjectSubsystem.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"
#include "Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "Window/KLDebugImGuiWindow.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Engine/Engine.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

bool UKLDebugImGuiWorldSubsystem::ShouldCreateSubsystem(UObject* _Outer) const
{
    return UKLDebugImGuiEngineSubsystem::Get() != nullptr;
}

void UKLDebugImGuiWorldSubsystem::Initialize(FSubsystemCollectionBase& _Collection)
{
    Super::Initialize(_Collection);

    const UKLDebugImGuiConfig& ImGuiConfig = UKLDebugImGuiConfig::Get();
    mImGuiWindow = ImGuiConfig.GetImGuiWindow();

    if (mImGuiWindow.IsValid())
    {
        FKLDebugImGuiWindow& ImGuiWindow = mImGuiWindow.GetMutable<FKLDebugImGuiWindow>();
        ImGuiWindow.Init();
    }
}

void UKLDebugImGuiWorldSubsystem::Deinitialize()
{
    Super::Deinitialize();

    FKLDebugImGuiWindow& ImGuiWindow = mImGuiWindow.GetMutable<FKLDebugImGuiWindow>();
    ImGuiWindow.Shutdown();
}

void UKLDebugImGuiWorldSubsystem::OnWorldBeginPlay(UWorld& _World)
{
    UKLDebugImGuiEngineSubsystem* EngineSusbsytem = UKLDebugImGuiEngineSubsystem::Get();
    if (!EngineSusbsytem)
    {
        ensureMsgf(false, TEXT("should not be possible. We check in ShouldCreateSubsystem that the engine subsystem exists"));
        return;
    }

    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = EngineSusbsytem->GetFeatureContainerManager();

    mImGuiTreeName = FString::Format(TEXT("{0} {1}"), { *GetDebugStringForWorld(&_World), *_World.GetName() });

    FString VisualizerName;
    VisualizerName.Reserve(200);

    mSubsystemsFeaturesVisualizer.Reserve(static_cast<int32>(EContainerType::COUNT));

    for (int32 i = 0; i < static_cast<int32>(EContainerType::COUNT); ++i)
    {
        const EContainerType ContainerType = static_cast<EContainerType>(i);
        switch (ContainerType)
        {
        case EContainerType::SELECTABLE_OBJECTS:
            continue;
        case EContainerType::ENGINE_SUBSYTSTEM:
            VisualizerName = TEXT("Engine Subsystems");
            break;
        case EContainerType::WORLD_SUBSYSTEM:
            VisualizerName = TEXT("World Subsystems");
            break;
        case EContainerType::EDITOR_SUBSYSTEM:
            VisualizerName = TEXT("Editor Subsystems");
            break;
        case EContainerType::GAME_INSTANCE_SUBSYSTEM:
            VisualizerName = TEXT("Game Instance Subsystems");
            break;
        case EContainerType::LOCAL_PLAYER_SUBSYSTEM:
            VisualizerName = TEXT("Local Player Subsystems");
            break;
        default:
            ensureMsgf(false, TEXT("we should not enter here"));
            continue;
        }

        TUniquePtr<FKLDebugImGuiFeatureVisualizerSubsystem> NewVisualizer = GetVisualizer(ContainerType, VisualizerName, FeatureContainerManager);
        if (NewVisualizer.IsValid())
        {
            mSubsystemsFeaturesVisualizer.Emplace(MoveTemp(NewVisualizer));
        }
    }
}

TUniquePtr<FKLDebugImGuiFeatureVisualizerSubsystem> UKLDebugImGuiWorldSubsystem::GetVisualizer(const EContainerType _ContainerType, const FString& _VisualizerName, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager) const
{
    const FKLDebugImGuiFeatureContainerBase& FeaturesContainer = _FeatureContainerManager.GetContainerMutable(_ContainerType);
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> Features;
    FeaturesContainer.GatherFeatures(*this, Features);

    if (Features.IsEmpty())
    {
        return nullptr;
    }

    return MakeUnique<FKLDebugImGuiFeatureVisualizerSubsystem>(FeaturesContainer, _VisualizerName, _ContainerType, MoveTemp(Features));
}

UKLDebugImGuiWorldSubsystem* UKLDebugImGuiWorldSubsystem::TryGetMutable(const UObject& _Object)
{
    const UWorld* World = GEngine->GetWorldFromContextObject(&_Object, EGetWorldErrorMode::ReturnNull);
    if (World)
    {
        return World->GetSubsystem<UKLDebugImGuiWorldSubsystem>();
    }

    return nullptr;
}

void UKLDebugImGuiWorldSubsystem::OnObjectSelected(UObject& _Object)
{
    const UKLDebugImGuiEngineSubsystem* EngineSusbsytem = UKLDebugImGuiEngineSubsystem::Get();
    if (!EngineSusbsytem->IsImGuiSystemEnabled())
    {
        return;
    }

    const int32 Index = mSelectedObjectsVisualizers.IndexOfByKey(_Object);
    if (Index != INDEX_NONE)
    {
        return;
    }

    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> Features;
    const FKLDebugImGuiFeaturesTypesContainerManager&       FeatureContainerManager = EngineSusbsytem->GetFeatureContainerManager();
    const FKLDebugImGuiFeatureContainerBase&                SelectedObjectFeatures  = FeatureContainerManager.GetContainer(EContainerType::SELECTABLE_OBJECTS);

    SelectedObjectFeatures.GatherFeatures(_Object, Features);
    mSelectedObjectsVisualizers.Emplace(SelectedObjectFeatures, EngineSusbsytem->GetOverlayMaterial(), _Object, MoveTemp(Features));
}

void UKLDebugImGuiWorldSubsystem::DrawImGui(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    ensureMsgf(&_CurrentWorldUpdated == GetWorld(), TEXT("we are updating the wrong world"));

    FKLDebugImGuiWindow& ImGuiWindow = mImGuiWindow.GetMutable<FKLDebugImGuiWindow>();
    ImGuiWindow.Update(_CurrentWorldUpdated);

    const ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings;
    if (!ImGui::Begin(TCHAR_TO_ANSI(*mImGuiTreeName), nullptr, WindowFlags))
    {
        ImGui::End();
        return;
    }

    ImGui::Separator();

    const ImGuiTabBarFlags TabFlags = ImGuiTabBarFlags_::ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyMask_;
    if (!ImGui::BeginTabBar("##tabs", TabFlags))
    {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabItem("FeaturesTree"))
    {
        ImGui::PushID(this);

        DrawImGuiVisualizers(_CurrentWorldUpdated, _ContainerManager);
        DrawImguiSelectedObjects(_CurrentWorldUpdated, _ContainerManager);

        ImGui::PopID();

        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::End();
}

void UKLDebugImGuiWorldSubsystem::Render(const UWorld& _CurrentWorldUpdated, const FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) const
{
    ensureMsgf(&_CurrentWorldUpdated == GetWorld(), TEXT("we are updating the wrong world"));

    const FKLDebugImGuiFeatureVisualizerRenderContext Context{ _CurrentWorldUpdated, _ContainerManager };
    for (const TUniquePtr<FKLDebugImGuiFeatureVisualizerSubsystem>& SubsystemVisualizer : mSubsystemsFeaturesVisualizer)
    {
        SubsystemVisualizer->Render(Context);
    }

    for (const FKLDebugImGuiFeatureVisualizerSelectableObject& ObjVisualizer : mSelectedObjectsVisualizers)
    {
        if (!ObjVisualizer.IsValid())
        {
            continue;
        }

        ObjVisualizer.Render(Context);
    }
}

void UKLDebugImGuiWorldSubsystem::DrawImGuiVisualizers(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) const
{
    const FKLDebugImGuiFeatureVisualizerImGuiContext Context{ _World, true, mOnFeaturesUpdatedDelegate, _ContainerManager };
    for (const TUniquePtr<FKLDebugImGuiFeatureVisualizerSubsystem>& SubsystemVisualizer : mSubsystemsFeaturesVisualizer)
    {
        SubsystemVisualizer->DrawImGui(Context);
    }
}

void UKLDebugImGuiWorldSubsystem::DrawImguiSelectedObjects(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    if (mSelectedObjectsVisualizers.IsEmpty())
    {
        return;
    }

    if (ImGui::TreeNode("Selected_Objects"))
    {
        DrawImGuiObjects(_World, true, _ContainerManager);
        ImGui::TreePop();
    }
    else
    {
        DrawImGuiObjects(_World, false, _ContainerManager);
    }
}

void UKLDebugImGuiWorldSubsystem::DrawImGuiObjects(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    ImGui::Indent();

    const FKLDebugImGuiFeatureVisualizerImGuiContext Context{ _World, _DrawTree, mOnFeaturesUpdatedDelegate, _ContainerManager };
    for (FKLDebugImGuiFeatureVisualizerSelectableObject& ObjVisualizer : mSelectedObjectsVisualizers)
    {
        if (!ObjVisualizer.IsValid())
        {
            continue;
        }

        ObjVisualizer.DrawImGui(Context);
    }

    ImGui::Unindent();

    for (int32 i = mSelectedObjectsVisualizers.Num() - 1; i >= 0; --i)
    {
        const FKLDebugImGuiFeatureVisualizerSelectableObject& ObjVisualizer = mSelectedObjectsVisualizers[i];
        if (!ObjVisualizer.IsValid() || !ObjVisualizer.ShouldKeepAlive())
        {
            mSelectedObjectsVisualizers.RemoveAt(i, 1, false);
        }
    }

}
