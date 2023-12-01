// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Container/Selectable/KLDebugImGuiFeatureContainer_Selectable.h"

#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"

bool FKLDebugImGuiFeatureContainer_Selectable::IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const
{
    return _Feature.IsDerivedFrom<IKLDebugImGuiFeatureInterface_Selectable>();
}

void FKLDebugImGuiFeatureContainer_Selectable::FinishGenerateFeatureChild()
{
    FKLDebugImGuiFeaturesIterator FeatureIterator(GetFeaturesIterator());
    mFilterTree.Init(GetFeaturesCount(), FeatureIterator);
}

void FKLDebugImGuiFeatureContainer_Selectable::GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
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
