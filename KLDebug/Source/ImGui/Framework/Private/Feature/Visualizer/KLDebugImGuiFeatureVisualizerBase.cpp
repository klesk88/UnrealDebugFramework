// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"

FKLDebugImGuiFeatureVisualizerBase::FKLDebugImGuiFeatureVisualizerBase(const FKLDebugImGuiFeatureContainerBase& _Container, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes)
    : mFeaturesIndexes(MoveTemp(_FeaturesIndexes))
    , mInterfaceType(_Container.GetContainerType())
{
    checkf(mInterfaceType != EImGuiInterfaceType::COUNT, TEXT("passed count as container"));

    FKLDebugImGuiSubsetFeaturesConstIterator Iterator(_Container.GetFeaturesSubsetConstIterator(mFeaturesIndexes));
    mTreeVisualizer.CreateTree(Iterator);
    mSelectedFeaturesIndexes.Reserve(30);
}

void FKLDebugImGuiFeatureVisualizerBase::Init(const FKLDebugImGuiFeatureContainerBase& _Container, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes)
{
    mFeaturesIndexes = MoveTemp(_FeaturesIndexes);
    mInterfaceType = _Container.GetContainerType();

    checkf(mInterfaceType != EImGuiInterfaceType::COUNT, TEXT("passed count as container"));

    FKLDebugImGuiSubsetFeaturesConstIterator Iterator(_Container.GetFeaturesSubsetConstIterator(mFeaturesIndexes));
    mTreeVisualizer.CreateTree(Iterator);
    mSelectedFeaturesIndexes.Reserve(30);
}

void FKLDebugImGuiFeatureVisualizerBase::DrawImGui(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context)
{
    if (_Context.GetShouldDrawTree())
    {
        DrawImGuiTree(_Context);
    }

    DrawImGuiFeaturesEnabled(_Context);
}