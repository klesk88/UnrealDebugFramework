#include "Feature/Container/SelectableObject/KLDebugImGuiFeatureContainer_SelectableObject.h"

#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"

bool FKLDebugImGuiFeatureContainer_SelectableObject::IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const
{
    return _Feature.IsDerivedFrom<IKLDebugImGuiFeatureInterface_SelectableObject>();
}

void FKLDebugImGuiFeatureContainer_SelectableObject::FinishGenerateFeatureChild()
{
    FKLDebugImGuiFeaturesIterator FeatureIterator(GetFeatureIteratorMutable());
    mFilterTree.Init(GetFeaturesCount(), FeatureIterator);
}
