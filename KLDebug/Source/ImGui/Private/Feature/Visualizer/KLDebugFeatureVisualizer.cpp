#include "Feature/Visualizer/KLDebugFeatureVisualizer.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"

// engine
#include "UObject/Object.h"

FKLDebugFeatureVisualizer::FKLDebugFeatureVisualizer(UObject& _Object, TArray<KL::Debug::Features::Types::FeatureIndex>&& _FeaturesIndexes)
    : mFeaturesIndexes(MoveTemp(_FeaturesIndexes))
    , mObject(&_Object)
{
}

bool FKLDebugFeatureVisualizer::operator==(const UObject& _Object) const
{
    return mObject.Get() == &_Object;
}

bool FKLDebugFeatureVisualizer::IsValid() const
{
    return mObject.IsValid();
}

void FKLDebugFeatureVisualizer::DrawImGui(FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const
{
    FKLDebugImGuiFeaturesIterator Iterator = _FeatureContainer.GetFeatureIterator(mFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
    }
}

void FKLDebugFeatureVisualizer::Render(FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const
{
    FKLDebugImGuiFeaturesIterator Iterator = _FeatureContainer.GetFeatureIterator(mFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
    }
}
