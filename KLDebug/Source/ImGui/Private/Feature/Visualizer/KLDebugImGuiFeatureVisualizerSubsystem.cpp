#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerSubsystem.h"

#include "Feature/Container/Iterators/KLDebugImGuiSubsetFeaturesIterator.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Interface/Private/KLDebugImGuiFeatureInterface_Subsystem.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"

FKLDebugImGuiFeatureVisualizerSubsystem::FKLDebugImGuiFeatureVisualizerSubsystem(const FKLDebugImGuiFeatureContainerBase& _Container, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes)
    : FKLDebugImGuiFeatureVisualizerBase(_Container, MoveTemp(_FeaturesIndexes))
{
}

void FKLDebugImGuiFeatureVisualizerSubsystem::DrawImGuiTree(const UWorld& _World)
{
    if (!ImGui::TreeNode(&mTreeVisualizer, TCHAR_TO_ANSI(*_World.GetName())))
    {
        return;
    }

    mTreeVisualizer.DrawImGuiTree(mSelectedFeaturesIndexes);

    ImGui::TreePop();
}

void FKLDebugImGuiFeatureVisualizerSubsystem::DrawImGuiFeaturesEnabled(const UWorld& _World, FKLDebugImGuiFeatureContainerBase& _FeatureContainer)
{
    auto Callback = [&_World](FKLDebugImGuiFeatureVisualizerIterator& Iterator, FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> bool {
        IKLDebugImGuiFeatureInterface_Subsystem& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Subsystem>();
        Interface.DrawImGui(_World, _Entry.GetIsEnableRef());
        return _Entry.IsEnable();
    };

    DrawImguiFeaturesEnabledCommon(_FeatureContainer, Callback);
}

void FKLDebugImGuiFeatureVisualizerSubsystem::Render(const UWorld& _World, FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const
{
    FKLDebugImGuiFeatureVisualizerConstIterator Iterator = _FeatureContainer.GetFeatureVisualizerConstIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        const IKLDebugImGuiFeatureInterface_Subsystem& Interface = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_Subsystem>();
        Interface.Render(_World);
    }
}