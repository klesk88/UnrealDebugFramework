// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "TreeBuilder/KLDebugImGuiTreeBuilderData.h"

// engine
#include "Containers/Array.h"

class IKLDebugImGuiFeatureInterface_Selectable;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiTreeSortedFeatures final : public FKLDebugImGuiTreeBuilderData
{
public:
    explicit FKLDebugImGuiTreeSortedFeatures(const KL::Debug::ImGui::Features::Types::FeatureIndex _Offset, const FName& _FeaturePath, const TArray<FName>& _Filters, const IKLDebugImGuiFeatureInterface_Selectable& _Feature);

    // FKLDebugImGuiTreeBuilderData
    UE_NODISCARD const TArray<FName>& GetPathTokens() const final;
    // FKLDebugImGuiTreeBuilderData

    UE_NODISCARD const IKLDebugImGuiFeatureInterface_Selectable& GetFeature() const;

private:
    const IKLDebugImGuiFeatureInterface_Selectable* mFeature = nullptr;
    TArray<FName> mFilters;
};

inline const TArray<FName>& FKLDebugImGuiTreeSortedFeatures::GetPathTokens() const
{
    return mFilters;
}

inline const IKLDebugImGuiFeatureInterface_Selectable& FKLDebugImGuiTreeSortedFeatures::GetFeature() const
{
    check(mFeature != nullptr);
    return *mFeature;
}
