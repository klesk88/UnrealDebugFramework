// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Container/Unique/KLDebugImGuiFeatureContainer_Unique.h"

// modules
#include "User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"

bool FKLDebugImGuiFeatureContainer_Unique::IsCorrectContainerForFeature(const IKLDebugImGuiFeatureInterfaceBase& _Feature) const
{
    return _Feature.IsDerivedFrom<IKLDebugImGuiFeatureInterface_Unique>();
}

void FKLDebugImGuiFeatureContainer_Unique::GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
{
    _OutFeaturesIndexes.Reserve(GetFeaturesCount());

    for (FKLDebugImGuiFeaturesConstIterator Iterator = GetFeaturesConstIterator(); Iterator; ++Iterator)
    {
        const IKLDebugImGuiFeatureInterface_Unique& Feature = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_Unique>();
        if (!Feature.DoesSupportObject(_Obj))
        {
            continue;
        }

        _OutFeaturesIndexes.Emplace(Iterator.GetFeatureDataIndex());
    }

    _OutFeaturesIndexes.Shrink();
}
