#include "Subsystems/KLDebugImGuiWorldSubsystem.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"
#include "Feature/Interface/Subsystem/KLDebugImGuiFeatureInterface_ObjectSubsystem.h"
#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Engine/Engine.h"
#include "Engine/World.h"

bool UKLDebugImGuiWorldSubsystem::ShouldCreateSubsystem(UObject* _Outer) const
{
    return UKLDebugImGuiEngineSubsystem::Get() != nullptr;
}

void UKLDebugImGuiWorldSubsystem::Initialize(FSubsystemCollectionBase& _Collection)
{
    UKLDebugImGuiEngineSubsystem* EngineSusbsytem = UKLDebugImGuiEngineSubsystem::Get();
    EngineSusbsytem->AddUpdatableSystem(*this);
    mIsInitialized = false;
}

void UKLDebugImGuiWorldSubsystem::Deinitialize()
{
    UKLDebugImGuiEngineSubsystem* EngineSusbsytem = UKLDebugImGuiEngineSubsystem::Get();
    if (EngineSusbsytem)
    {
        EngineSusbsytem->RemoveUpdatableSystem(*this, mIsInitialized);
    }
}

void UKLDebugImGuiWorldSubsystem::Initialize(FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
{
    // Initialize the tree here. At this point all the world subsystems should have been spawn already. The same is not true if I do it in
    // Initialize (note I could use here PostInitialize but all the other subsystems do not have such feature so for consistency use the same)

    mIsInitialized                                          = true;
    const FKLDebugImGuiFeatureContainerBase& WorldContainer = _FeatureContainerManager.GetContainer(EContainerType::WORLD_SUBSYSTEM);

    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> Features;
    UWorld&                                     World = *GetWorld();
    WorldContainer.GatherFeatures(World, Features);

    if (Features.IsEmpty())
    {
        return;
    }

    mWorldVisualizer = MakeUnique<FKLDebugImGuiFeatureVisualizerSubsystem>(WorldContainer, MoveTemp(Features));
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

void UKLDebugImGuiWorldSubsystem::Update(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    if (&_CurrentWorldUpdated != GetWorld())
    {
        return;
    }
}

void UKLDebugImGuiWorldSubsystem::DrawImGui(const UWorld& _CurrentWorldUpdated, const bool _TabOpen, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    if (&_CurrentWorldUpdated != GetWorld())
    {
        return;
    }

    ImGui::PushID(this);

    if (mWorldVisualizer.IsValid())
    {
        if (ImGui::TreeNode("World"))
        {
            DrawImGuiWorld(_CurrentWorldUpdated, true, _ContainerManager);
            ImGui::TreePop();
        }
        else
        {
            DrawImGuiWorld(_CurrentWorldUpdated, false, _ContainerManager);
        }
    }

    if (!mSelectedObjectsVisualizers.IsEmpty())
    {
        if (ImGui::TreeNode("Selected_Objects"))
        {
            DrawImGuiObjects(_CurrentWorldUpdated, true, _ContainerManager);
            ImGui::TreePop();
        }
        else
        {
            DrawImGuiObjects(_CurrentWorldUpdated, false, _ContainerManager);
        }
    }

    ImGui::PopID();
}

void UKLDebugImGuiWorldSubsystem::DrawImGuiWorld(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    ImGui::Indent();

    FKLDebugImGuiFeatureContainerBase& WorldContainer = _ContainerManager.GetContainerMutable(EContainerType::WORLD_SUBSYSTEM);
    mWorldVisualizer->DrawImGui(_World, _DrawTree, WorldContainer);
    mWorldVisualizer->Render(_World, WorldContainer);

    ImGui::Unindent();
}

void UKLDebugImGuiWorldSubsystem::DrawImGuiObjects(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    ImGui::Indent();

    FKLDebugImGuiFeatureContainerBase& SelectableObjFeatures = _ContainerManager.GetContainerMutable(EContainerType::SELECTABLE_OBJECTS);
    for (FKLDebugImGuiFeatureVisualizerSelectableObject& ObjVisualizer : mSelectedObjectsVisualizers)
    {
        if (!ObjVisualizer.IsValid())
        {
            continue;
        }

        ObjVisualizer.DrawImGui(_World, _DrawTree, SelectableObjFeatures);
        ObjVisualizer.Render(_World, SelectableObjFeatures);
    }

    for (int32 i = mSelectedObjectsVisualizers.Num() - 1; i >= 0; --i)
    {
        const FKLDebugImGuiFeatureVisualizerSelectableObject& ObjVisualizer = mSelectedObjectsVisualizers[i];
        if (!ObjVisualizer.IsValid() || !ObjVisualizer.ShouldKeepAlive())
        {
            mSelectedObjectsVisualizers.RemoveAt(i, 1, false);
        }
    }

    ImGui::Unindent();
}
