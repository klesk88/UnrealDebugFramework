#include "Subsystems/KLDebugImGuiWorldSubsystem.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

// engine
#include "Engine/Engine.h"
#include "Engine/World.h"

bool UKLDebugImGuiWorldSubsystem::ShouldCreateSubsystem(UObject* _Outer) const
{
    return UKLDebugImGuiEngineSubsystem::Get() != nullptr;
}

void UKLDebugImGuiWorldSubsystem::PostInitialize()
{
    InitWorldVisualizer();
}

void UKLDebugImGuiWorldSubsystem::Deinitialize()
{
    UKLDebugImGuiEngineSubsystem* EngineSusbsytem = UKLDebugImGuiEngineSubsystem::Get();
    if (EngineSusbsytem)
    {
        EngineSusbsytem->RemoveUpdatableSystem(*this);
    }
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

void UKLDebugImGuiWorldSubsystem::InitWorldVisualizer()
{
    UKLDebugImGuiEngineSubsystem* EngineSusbsytem = UKLDebugImGuiEngineSubsystem::Get();
    EngineSusbsytem->AddUpdatableSystem(*this);

    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = EngineSusbsytem->GetFeatureContainerManager();
    const FKLDebugImGuiFeatureContainerBase&          WorldContainer          = FeatureContainerManager.GetContainer(EContainerType::WORLD_SUBSYSTEM);

    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> Features;
    UWorld&                                                 World = *GetWorld();
    WorldContainer.GatherFeatures(World, Features);

    if (Features.IsEmpty())
    {
        return;
    }

    mWorldVisualizer = MakeUnique<FKLDebugImGuiFeatureVisualizer>(World, MoveTemp(Features));
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

    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = EngineSusbsytem->GetFeatureContainerManager();
    const FKLDebugImGuiFeatureContainerBase&          SelectedObjectFeatures  = FeatureContainerManager.GetContainer(EContainerType::SELECTABLE_OBJECTS);

    SelectedObjectFeatures.GatherFeatures(_Object, Features);
    mSelectedObjectsVisualizers.Emplace(_Object, MoveTemp(Features));
}

void UKLDebugImGuiWorldSubsystem::Update(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    if (&_CurrentWorldUpdated != GetWorld())
    {
        return;
    }

    UpateWorldVisualizer(_ContainerManager);
    UpdateSelectedObjectsVisualizers(_ContainerManager);
}

void UKLDebugImGuiWorldSubsystem::UpateWorldVisualizer(FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) const
{
    if (!mWorldVisualizer.IsValid())
    {
        return;
    }

    FKLDebugImGuiFeatureContainerBase& WorldContainer = _ContainerManager.GetContainerMutable(EContainerType::WORLD_SUBSYSTEM);
    mWorldVisualizer->DrawImGui(WorldContainer);
    mWorldVisualizer->Render(WorldContainer);
}

void UKLDebugImGuiWorldSubsystem::UpdateSelectedObjectsVisualizers(FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    FKLDebugImGuiFeatureContainerBase& SelectableObjFeatures = _ContainerManager.GetContainerMutable(EContainerType::SELECTABLE_OBJECTS);

    for (const FKLDebugImGuiFeatureVisualizer& ObjVisualizer : mSelectedObjectsVisualizers)
    {
        if (!ObjVisualizer.IsValid())
        {
            continue;
        }

        ObjVisualizer.DrawImGui(SelectableObjFeatures);
        ObjVisualizer.Render(SelectableObjFeatures);
    }
}
