#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

#include "Feature/Container/Iterators/KLDebugImGuiSubsetFeaturesIterator.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"

FKLDebugImGuiFeatureVisualizerBase::FKLDebugImGuiFeatureVisualizerBase(const FKLDebugImGuiFeatureContainerBase& _Container, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes)
    : mFeaturesIndexes(MoveTemp(_FeaturesIndexes))
{
    FKLDebugImGuiSubsetFeaturesConstIterator Iterator(_Container.GetFeaturesSubsetConstIterator(mFeaturesIndexes));
    mTreeVisualizer.CreateTree(Iterator);
    mSelectedFeaturesIndexes.Reserve(30);
}

void FKLDebugImGuiFeatureVisualizerBase::DrawImGui(const UWorld& _World, const bool _DrawTree, FKLDebugImGuiFeatureContainerBase& _FeatureContainer)
{
    if (_DrawTree)
    {
        DrawImGuiTree(_World);
    }

    DrawImGuiFeaturesEnabled(_World, _FeatureContainer);
}
