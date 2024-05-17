// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerBase.h"

#include "Config/KLDebugImGuiConfig.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"

// modules
#include "User/Framework/Internal/Feature/Interface/Input/KLDebugFeatureDrawCanvasInput_Base.h"

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
        IKLDebugFeatureInterfaceBase& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugFeatureInterfaceBase>();
        KL::Debug::Feature::Helpers::OnFeatureUnselected(_World, _OwnerObject, Interface);
    }
}

void FKLDebugImGuiFeatureVisualizerBase::DrawImGui(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem)
{
    if (_Context.GetShouldDrawTree())
    {
        DrawImGuiTree(_Context);
    }

    DrawImGuiFeaturesEnabled(_Context, _RequiredExternalSystem);
}

void FKLDebugImGuiFeatureVisualizerBase::InitCommonCanvasInput(UWorld& _World, FKLDebugFeatureDrawCanvasInput_Base& _Input)
{
    const UKLDebugImGuiConfig& Config = UKLDebugImGuiConfig::Get();
    const FKLDebugImGuiConfig_Canvas& CanvasConfig = Config.GetCanvasConfig();

    _Input.CursorX = _Input.DefaultX = CanvasConfig.GetBorderOffsetX();
    _Input.CursorY = _Input.DefaultY = CanvasConfig.GetBorderOffsetY();
    _Input.World = &_World;
}

void FKLDebugImGuiFeatureVisualizerBase::GetSelectedFeaturesIndexes(TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _SelectedFeatures) const
{
    _SelectedFeatures.Reset();
    _SelectedFeatures.Reserve(mSelectedFeaturesIndexes.Num());
    for (const FKLDebugImGuiFeatureVisualizerEntry& Entry : mSelectedFeaturesIndexes)
    {
        _SelectedFeatures.Emplace(Entry.GetFeatureIndex());
    }
}
