#pragma once

#include "TreeBuilder/KLDebugImGuiTreeBuilderData.h"

// engine
#include "Containers/Array.h"

class IKLDebugImGuiFeatureInterface_SelectableObject;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiTreeSortedFeatures final : public FKLDebugImGuiTreeBuilderData
{
public:
    explicit FKLDebugImGuiTreeSortedFeatures(const KL::Debug::ImGui::Features::Types::FeatureIndex _Offset, const FName& _FeaturePath, const TArray<FName>& _Filters, const IKLDebugImGuiFeatureInterface_SelectableObject& _Feature);

    // FKLDebugImGuiTreeBuilderData
    UE_NODISCARD const TArray<FName>& GetPathTokens() const final;
    // FKLDebugImGuiTreeBuilderData

    UE_NODISCARD const IKLDebugImGuiFeatureInterface_SelectableObject& GetFeature() const;

private:
    const IKLDebugImGuiFeatureInterface_SelectableObject* mFeature = nullptr;
    TArray<FName>                                         mFilters;
};

inline const TArray<FName>& FKLDebugImGuiTreeSortedFeatures::GetPathTokens() const
{
    return mFilters;
}

inline const IKLDebugImGuiFeatureInterface_SelectableObject& FKLDebugImGuiTreeSortedFeatures::GetFeature() const
{
    check(mFeature != nullptr);
    return *mFeature;
}
