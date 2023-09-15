#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerSubsystem.h"

#include "Feature/Container/Iterators/KLDebugImGuiSubsetFeaturesIterator.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Interface/Private/KLDebugImGuiFeatureInterface_Subsystem.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"

FKLDebugImGuiFeatureVisualizerSubsystem::FKLDebugImGuiFeatureVisualizerSubsystem(const FKLDebugImGuiFeatureContainerBase& _Container, const FString& _TreeName, const EContainerType _ContainerType, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes)
    : FKLDebugImGuiFeatureVisualizerBase(_Container, MoveTemp(_FeaturesIndexes))
    , mTreeName(_TreeName)
    , mContainerType(_ContainerType)
{
    checkf(mContainerType != EContainerType::COUNT, TEXT("must be initialized correctly"));
}

void FKLDebugImGuiFeatureVisualizerSubsystem::DrawImGuiTree(const UWorld& _World)
{
    if (ImGui::TreeNode(this, TCHAR_TO_ANSI(*mTreeName)))
    {
        mTreeVisualizer.DrawImGuiTree(mSelectedFeaturesIndexes);
        ImGui::TreePop();
    }
}

void FKLDebugImGuiFeatureVisualizerSubsystem::DrawImGuiFeaturesEnabled(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
{
    auto Callback = [&_World](FKLDebugImGuiFeatureVisualizerIterator& Iterator, FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> bool {
        IKLDebugImGuiFeatureInterface_Subsystem& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Subsystem>();
        Interface.DrawImGui(_World, _Entry.GetIsEnableRef());
        return _Entry.IsEnable();
    };

    FKLDebugImGuiFeatureContainerBase& FeaturesContainer = _FeatureContainerManager.GetContainerMutable(mContainerType);
    DrawImguiFeaturesEnabledCommon(FeaturesContainer, Callback);
}

void FKLDebugImGuiFeatureVisualizerSubsystem::Render(const UWorld& _World, const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager) const
{
    const FKLDebugImGuiFeatureContainerBase& FeaturesContainer = _FeatureContainerManager.GetContainer(mContainerType);
    FKLDebugImGuiFeatureVisualizerConstIterator Iterator = FeaturesContainer.GetFeatureVisualizerConstIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        const IKLDebugImGuiFeatureInterface_Subsystem& Interface = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_Subsystem>();
        Interface.Render(_World);
    }
}