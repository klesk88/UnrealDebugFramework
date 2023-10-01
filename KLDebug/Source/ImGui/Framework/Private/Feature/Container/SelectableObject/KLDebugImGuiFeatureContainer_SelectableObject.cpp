#include "Feature/Container/SelectableObject/KLDebugImGuiFeatureContainer_SelectableObject.h"

#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"

bool FKLDebugImGuiFeatureContainer_SelectableObject::IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const
{
    return _Feature.IsDerivedFrom<IKLDebugImGuiFeatureInterface_SelectableObject>();
}

void FKLDebugImGuiFeatureContainer_SelectableObject::FinishGenerateFeatureChild()
{
    FKLDebugImGuiFeaturesIterator FeatureIterator(GetFeaturesIterator());
    mFilterTree.Init(GetFeaturesCount(), FeatureIterator);
}

void FKLDebugImGuiFeatureContainer_SelectableObject::GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
{
    if (mFilterTree.HasFilters())
    {
        mFilterTree.GatherFeatures(_Obj, _OutFeaturesIndexes);
        return;
    }

    UE_LOG(LogKL_Debug, Error, TEXT("No filters available"));
    for (int32 i = 0; i < GetFeaturesData().Num(); ++i)
    {
        _OutFeaturesIndexes.Emplace(i);
    }
}
