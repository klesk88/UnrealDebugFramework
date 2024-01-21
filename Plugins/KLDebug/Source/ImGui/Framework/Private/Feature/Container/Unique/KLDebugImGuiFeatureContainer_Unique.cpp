// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Container/Unique/KLDebugImGuiFeatureContainer_Unique.h"

// modules
#include "User/Framework/Public/Feature/Interface/Unique/KLDebugFeatureInterface_Unique.h"

// engine
#include "Engine/World.h"

bool FKLDebugImGuiFeatureContainer_Unique::IsCorrectContainerForFeature(const IKLDebugFeatureInterfaceBase& _Feature) const
{
    return _Feature.IsDerivedFrom<IKLDebugFeatureInterface_Unique>();
}

void FKLDebugImGuiFeatureContainer_Unique::GatherFeaturesChild(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
{
    const UWorld* World = CastChecked<const UWorld>(&_Obj);

    _OutFeaturesIndexes.Reserve(GetFeaturesCount());

    for (FKLDebugImGuiFeaturesConstIterator Iterator = GetFeaturesConstIterator(); Iterator; ++Iterator)
    {
        const IKLDebugFeatureInterface_Unique& Feature = Iterator.GetFeatureInterfaceCasted<IKLDebugFeatureInterface_Unique>();
        if (!Feature.DoesSupportWorld(*World))
        {
            continue;
        }

        _OutFeaturesIndexes.Emplace(Iterator.GetFeatureDataIndex());
    }

    _OutFeaturesIndexes.Shrink();
}
