#pragma once

#include "Feature/Container/KLDebugImGuiFeatureData.h"
#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"

class IKLDebugImGuiFeatureInterfaceBase;

class KLDEBUGIMGUI_API FKLDebugImGuiFeaturesIterator_SelectableObject final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeaturesIterator_SelectableObject(const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _FeaturesIndexes, const TArray<FKLDebugImGuiFeatureData>& _FeatureData, TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& _FeaturesPool);

    explicit operator bool() const;
    void     operator++();

    template<typename FeatureInterfaceType>
    UE_NODISCARD const FeatureInterfaceType& GetFeatureInterfaceCasted() const;
    template<typename FeatureInterfaceType>
    UE_NODISCARD FeatureInterfaceType& GetFeatureInterfaceCastedMutable() const;

private:
    UE_NODISCARD IKLDebugImGuiFeatureInterfaceBase& GetFeatureMutable() const;

private:
    // the data contains the offset between features so that we can retrieve them correctly from the pool. Each entry is the
    // start address index of a new feature inside mFeaturesPool.
    const TArray<FKLDebugImGuiFeatureData>& mFeatureData;

    const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& mFeaturesIndexes;

    // pool of features. This is a byte array and all features are allocated inside of it
    // in this way they are all packed in memory close together
    TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& mFeaturesPool;

    uint32 mIndex = 0;
};

inline FKLDebugImGuiFeaturesIterator_SelectableObject::operator bool() const
{
    return mIndex < static_cast<uint32>(mFeaturesIndexes.Num());
}

inline void FKLDebugImGuiFeaturesIterator_SelectableObject::operator++()
{
    ++mIndex;
}

template<typename FeatureInterfaceType>
inline FeatureInterfaceType& FKLDebugImGuiFeaturesIterator_SelectableObject::GetFeatureInterfaceCastedMutable() const
{
    checkf(GetFeatureMutable().IsDerivedFrom<FeatureInterfaceType>(), TEXT("casting to wrong type"));
    return reinterpret_cast<FeatureInterfaceType&>(GetFeatureMutable());
}

template<typename FeatureInterfaceType>
inline const FeatureInterfaceType& FKLDebugImGuiFeaturesIterator_SelectableObject::GetFeatureInterfaceCasted() const
{
    return GetFeatureInterfaceCastedMutable<FeatureInterfaceType>();
}

inline IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeaturesIterator_SelectableObject::GetFeatureMutable() const
{
    const KL::Debug::ImGui::Features::Types::FeatureIndex Offset = mFeaturesIndexes[mIndex];
    return *reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[Offset]);
}
