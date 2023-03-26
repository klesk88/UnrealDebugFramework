#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"

// engine
#include "UObject/Object.h"

FKLDebugImGuiFeatureVisualizer::FKLDebugImGuiFeatureVisualizer(UObject& _Object, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes)
    : mFeaturesIndexes(MoveTemp(_FeaturesIndexes))
    , mObject(&_Object)
{
}

bool FKLDebugImGuiFeatureVisualizer::operator==(const UObject& _Object) const
{
    return mObject.Get() == &_Object;
}

bool FKLDebugImGuiFeatureVisualizer::IsValid() const
{
    return mObject.IsValid();
}

void FKLDebugImGuiFeatureVisualizer::DrawImGuiTree()
{
    mTreeVisualizer.DrawImGuiTree();
}

void FKLDebugImGuiFeatureVisualizer::DrawImGui(FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const
{
}

void FKLDebugImGuiFeatureVisualizer::Render(FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const
{
}
