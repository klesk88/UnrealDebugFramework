// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"

// modules
#include "ImGui/User/Public/Feature/Interface/Canvas/KLDebugImGuiFeatureCanvasInput.h"

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

void FKLDebugImGuiFeatureVisualizerBase::Shutdown(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, UObject* _OwnerObject)
{
    if (mInterfaceType == EImGuiInterfaceType::COUNT)
    {
        return;
    }

    FKLDebugImGuiFeatureContainerBase& FeatureContainer = _FeatureContainerManager.GetContainerMutable(mInterfaceType);
    FKLDebugImGuiFeatureVisualizerIterator Iterator = FeatureContainer.GetFeatureVisualizerIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        IKLDebugImGuiFeatureInterfaceBase& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterfaceBase>();
        KL::Debug::Feature::Helpers::OnFeatureUnselected(_World, _OwnerObject, Interface);
    }
}

void FKLDebugImGuiFeatureVisualizerBase::DrawImGui(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context, bool& _RequireCanvasDrawing)
{
    _RequireCanvasDrawing = false;

    if (_Context.GetShouldDrawTree())
    {
        DrawImGuiTree(_Context);
    }

    DrawImGuiFeaturesEnabled(_Context, _RequireCanvasDrawing);
}

void FKLDebugImGuiFeatureVisualizerBase::DrawCanvas(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const UObject& _OwnerObject, FKLDebugImGuiFeatureCanvasInput& _Input) const
{
    // not optimal. We could store the indices of just the features which require canvas update and iterate on them.
    // However I think we should have really few windows open at any point in time so it should not be really a big deal.
    // If in the future tough it becomes a problem tough revisit this

    _Input.SetOwnerObject(_OwnerObject);

    const FKLDebugImGuiFeatureContainerBase& FeatureContainer = _FeatureContainerManager.GetContainer(mInterfaceType);
    FKLDebugImGuiFeatureVisualizerConstIterator Iterator = FeatureContainer.GetFeatureVisualizerConstIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        const IKLDebugImGuiFeatureInterfaceBase& Interface = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
        if (Interface.RequireCanvasUpdate())
        {
            _Input.SetContext(Iterator.GetEntryData().TryGetFeatureContextMutable());
            Interface.DrawOnCanvas(_Input);
        }
    }
}
