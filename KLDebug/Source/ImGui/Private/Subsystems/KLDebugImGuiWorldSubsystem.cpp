#include "Subsystems/KLDebugImGuiWorldSubsystem.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"
#include "Feature/Interface/Subsystem/KLDebugImGuiFeatureInterface_ObjectSubsystem.h"
#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Engine/Engine.h"
#include "Engine/World.h"

bool UKLDebugImGuiWorldSubsystem::ShouldCreateSubsystem(UObject* _Outer) const
{
    return UKLDebugImGuiEngineSubsystem::Get() != nullptr;
}

void UKLDebugImGuiWorldSubsystem::PostInitialize()
{
    UKLDebugImGuiEngineSubsystem* EngineSusbsytem = UKLDebugImGuiEngineSubsystem::Get();
    EngineSusbsytem->AddUpdatableSystem(*this);
}

void UKLDebugImGuiWorldSubsystem::Deinitialize()
{
    UKLDebugImGuiEngineSubsystem* EngineSusbsytem = UKLDebugImGuiEngineSubsystem::Get();
    if (EngineSusbsytem)
    {
        EngineSusbsytem->RemoveUpdatableSystem(*this);
    }
}

void UKLDebugImGuiWorldSubsystem::Initialize(FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
{
    const FKLDebugImGuiFeatureContainerBase& WorldContainer = _FeatureContainerManager.GetContainer(EContainerType::WORLD_SUBSYSTEM);

    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> Features;
    UWorld&                                                 World = *GetWorld();
    WorldContainer.GatherFeatures(World, Features);

    if (Features.IsEmpty())
    {
        return;
    }

    mWorldVisualizer = MakeUnique<FKLDebugImGuiFeatureVisualizer>(World, MoveTemp(Features));
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

    UpdateSelectedObjectsVisualizers(_ContainerManager);
}

void UKLDebugImGuiWorldSubsystem::DrawImGui(const UWorld& _CurrentWorldUpdated, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager)
{
    if (&_CurrentWorldUpdated != GetWorld())
    {
        return;
    }

    if (!ImGui::BeginTabBar(TCHAR_TO_ANSI(*_CurrentWorldUpdated.GetName()), ImGuiTabBarFlags_::ImGuiTabBarFlags_None))
    {
        return;
    }

    DrawImGuiWorld(_CurrentWorldUpdated, _ContainerManager);

    ImGui::EndTabBar();
}

void UKLDebugImGuiWorldSubsystem::DrawImGuiWorld(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager) const
{
    FKLDebugImGuiFeatureContainerBase& WorldContainer = _ContainerManager.GetContainerMutable(EContainerType::WORLD_SUBSYSTEM);
    FKLDebugImGuiFeaturesIterator      Iterator       = WorldContainer.GetFeaturesIterator();
    for (; Iterator; ++Iterator)
    {
        IKLDebugImGuiFeatureInterface_ObjectSubsystem& ObjectSubsystem = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_ObjectSubsystem>();
        if (!ObjectSubsystem.DoesSupportObject(_World))
        {
            continue;
        }

        ObjectSubsystem.DrawImGui(_World);
    }
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
