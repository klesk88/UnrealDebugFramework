#pragma once

#include "Feature/KLDebugFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class IKLDebugImGuiFeatureInterface_SelectableObject;

class KLDEBUGIMGUI_API FKLDebugImGuiTreeSortedFeatures final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiTreeSortedFeatures(const KL::Debug::Features::Types::FeatureIndex _Offset, const IKLDebugImGuiFeatureInterface_SelectableObject& _Feature);

    void SetFilters(const TArray<FName>& _Filters, const FName& _FullFilter);

    UE_NODISCARD const IKLDebugImGuiFeatureInterface_SelectableObject& GetFeature() const;
    UE_NODISCARD const TArray<FName>& GetFilters() const;
    UE_NODISCARD const FName&         GetFullFilter() const;
    UE_NODISCARD KL::Debug::Features::Types::FeatureIndex GetFeatureOffset() const;

private:
    KL::Debug::Features::Types::FeatureIndex              mOffset  = 0;
    const IKLDebugImGuiFeatureInterface_SelectableObject* mFeature = nullptr;
    TArray<FName>                                         mFilters;
    FName                                                 mFullFilter;
};

inline FKLDebugImGuiTreeSortedFeatures::FKLDebugImGuiTreeSortedFeatures(const KL::Debug::Features::Types::FeatureIndex _Offset, const IKLDebugImGuiFeatureInterface_SelectableObject& _Feature)
    : mOffset(_Offset)
    , mFeature(&_Feature)
{
}

inline void FKLDebugImGuiTreeSortedFeatures::SetFilters(const TArray<FName>& _Filters, const FName& _FullFilter)
{
    mFilters    = _Filters;
    mFullFilter = _FullFilter;
}

inline const IKLDebugImGuiFeatureInterface_SelectableObject& FKLDebugImGuiTreeSortedFeatures::GetFeature() const
{
    check(mFeature != nullptr);
    return *mFeature;
}

inline const TArray<FName>& FKLDebugImGuiTreeSortedFeatures::GetFilters() const
{
    return mFilters;
}

inline const FName& FKLDebugImGuiTreeSortedFeatures::GetFullFilter() const
{
    return mFullFilter;
}

inline KL::Debug::Features::Types::FeatureIndex FKLDebugImGuiTreeSortedFeatures::GetFeatureOffset() const
{
    return mOffset;
}