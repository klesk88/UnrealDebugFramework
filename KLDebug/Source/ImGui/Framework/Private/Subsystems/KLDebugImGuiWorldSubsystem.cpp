#include "Subsystems/KLDebugImGuiWorldSubsystem.h"

#include "Config/KLDebugImGuiConfig.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
#include "Feature/Input/KLDebugImGuiGatherFeatureInput.h"

#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"
#include "Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "Window/KLDebugImGuiWindow.h"

//modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Engine/Engine.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Stats/Stats2.h"

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
    const UKLDebugImGuiEngineSubsystem* EngineSusbsytem = UKLDebugImGuiEngineSubsystem::Get();
    if (!EngineSusbsytem)
    {
        ensureMsgf(false, TEXT("should not be possible. We check in ShouldCreateSubsystem that the engine subsystem exists"));
        return;
    }

    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = EngineSusbsytem->GetFeatureContainerManager();

    mImGuiTreeName = FString::Format(TEXT("{0} {1}"), { *GetDebugStringForWorld(&_World), *_World.GetName() });

    const FKLDebugImGuiFeatureContainerBase& FeaturesContainer = FeatureContainerManager.GetContainerMutable(EImGuiInterfaceType::UNIQUE);
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> Features;
    FeaturesContainer.GatherFeatures(*this, Features);

    if (Features.IsEmpty())
    {
        return;
    }

    mUniqueFeaturesVisualizer.Init(FeaturesContainer, TEXT("Systems"), MoveTemp(Features));
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
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_OnObjectSelected);

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
    const FKLDebugImGuiFeatureContainerBase&                SelectedObjectFeatures  = FeatureContainerManager.GetContainer(EImGuiInterfaceType::SELECTABLE);

    SelectedObjectFeatures.GatherFeatures(_Object, Features);
    mSelectedObjectsVisualizers.Emplace(SelectedObjectFeatures, EngineSusbsytem->GetOverlayMaterial(), _Object, MoveTemp(Features));
}

void UKLDebugImGuiWorldSubsystem::TryGatherFeatureAndContext(FKLDebugImGuiGatherFeatureInput& _Input) const
{
    const FKLDebugImGuiFeaturesTypesContainerManager& ContainerManager = _Input.GetContainerManager();
    FKLDebugImGuiFeatureContainerBase& Container = ContainerManager.GetContainerMutable(_Input.GetContainerType());
    IKLDebugImGuiFeatureInterfaceBase& Interface = Container.GetFeatureMutable(_Input.GetFeatureIndex());
    FKLDebugImGuiFeatureContext_Base* FeatureContext = nullptr;

    if (const UObject* Object = _Input.TryGetObject())
    {
        const FKLDebugImGuiFeatureVisualizer_Selectable* ObjectFeatures = mSelectedObjectsVisualizers.FindByKey(*Object);
        const FKLDebugImGuiFeatureVisualizerEntry* VisualizerEntry = ObjectFeatures ? ObjectFeatures->TryGetSelectedFeature(_Input.GetFeatureIndex()) : nullptr;
        FeatureContext = VisualizerEntry ? VisualizerEntry->TryGetFeatureContextMutable() : nullptr;
    }
    else
    {

    }

    _Input.SetFeatureData(Interface, FeatureContext);
}

void UKLDebugImGuiWorldSubsystem::DrawImGui(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_DrawImGui);

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
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_Render);

    ensureMsgf(&_CurrentWorldUpdated == GetWorld(), TEXT("we are updating the wrong world"));

    const FKLDebugImGuiFeatureVisualizerRenderContext Context{ _CurrentWorldUpdated, _ContainerManager };
    
    if (mUniqueFeaturesVisualizer.IsValid())
    {
        mUniqueFeaturesVisualizer.Render(Context);
    }

    for (const FKLDebugImGuiFeatureVisualizer_Selectable& ObjVisualizer : mSelectedObjectsVisualizers)
    {
        if (!ObjVisualizer.IsValid())
        {
            continue;
        }

        ObjVisualizer.Render(Context);
    }
}

void UKLDebugImGuiWorldSubsystem::DrawImGuiVisualizers(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_DrawImGuiVisualizers);

    if (!mUniqueFeaturesVisualizer.IsValid())
    {
        return;
    }

    const FKLDebugImGuiFeatureVisualizerImGuiContext Context{ _World, true, mOnFeaturesUpdatedDelegate, _ContainerManager };
    mUniqueFeaturesVisualizer.DrawImGui(Context);
}

void UKLDebugImGuiWorldSubsystem::DrawImguiSelectedObjects(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiWorldSubsystem_DrawImguiSelectedObjects);

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
    for (FKLDebugImGuiFeatureVisualizer_Selectable& ObjVisualizer : mSelectedObjectsVisualizers)
    {
        if (!ObjVisualizer.IsValid())
        {
            continue;
        }

        ObjVisualizer.DrawImGui(Context);
    }

    ImGui::Unindent();

    const EImGuiInterfaceType SelectableContainerType = EImGuiInterfaceType::SELECTABLE;
    const FKLDebugImGuiFeatureContainerBase& SelectableObjectsContainer = _ContainerManager.GetContainer(SelectableContainerType);

    for (int32 i = mSelectedObjectsVisualizers.Num() - 1; i >= 0; --i)
    {
        const FKLDebugImGuiFeatureVisualizer_Selectable& ObjVisualizer = mSelectedObjectsVisualizers[i];
        if (!ObjVisualizer.IsValid() || !ObjVisualizer.ShouldKeepAlive())
        {
            if (mOnFeaturesUpdatedDelegate.IsBound() && !ObjVisualizer.GetFeaturesIndexes().IsEmpty())
            {
                FKLDebugImGuiSubsetFeaturesConstIterator Iterator = SelectableObjectsContainer.GetFeaturesSubsetConstIterator(ObjVisualizer.GetFeaturesIndexes());
                FKLDebugImGuiFeatureStatusUpdateData DelegateData{ false, SelectableContainerType, ObjVisualizer.GetObjectKey(), Iterator };
                DelegateData.SetFullyRemoved();
                mOnFeaturesUpdatedDelegate.Broadcast(DelegateData);
            }

            mSelectedObjectsVisualizers.RemoveAt(i, 1, false);
        }
    }
}
