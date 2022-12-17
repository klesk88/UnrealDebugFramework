#include "Feature/Container/KLDebugImGuiFeatureContainer.h"

#include "Feature/Interface/KLDebugImGuiFeatureInterface.h"
#include "Feature/Manager/KLDebugImGuiFeatureManager.h"
#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

IKLDebugImGuiFeatureInterface& FKLDebugImGuiFeatureContainer::GetFeatureMutable(const uint32 _Offset)
{
    checkf(_Offset < static_cast<uint32>(mFeaturesPool.Num()), TEXT("Out of bounds"));
    return *reinterpret_cast<IKLDebugImGuiFeatureInterface*>(&mFeaturesPool[_Offset]);
}

const IKLDebugImGuiFeatureInterface& FKLDebugImGuiFeatureContainer::GetFeature(const uint32 _Offset) const
{
    checkf(_Offset < static_cast<uint32>(mFeaturesPool.Num()), TEXT("Out of bounds"));
    return *reinterpret_cast<const IKLDebugImGuiFeatureInterface*>(&mFeaturesPool[_Offset]);
}

void FKLDebugImGuiFeatureContainer::Initialize()
{
    GatherFeatures();
}

void FKLDebugImGuiFeatureContainer::GatherFeatures()
{
    const FKLDebugImGuiFeatureManager& FeatureManager = FKLDebugImGuiFeatureManager::Get();
    mFeaturesOffset.Reserve(FeatureManager.GetEntryCount());
    mFeaturesPool.AddZeroed(FeatureManager.GetTotalSizeRequired());

    FKLDebugImGuiFeatureManagerEntryBase* Entry       = FeatureManager.GetStartEntry();
    uint32                                OffsetIndex = 0;

    while (Entry)
    {
        mFeaturesOffset.Emplace(OffsetIndex);
        IKLDebugImGuiFeatureInterface& DebugWindow = Entry->AllocateInPlace(static_cast<void*>(&mFeaturesPool[OffsetIndex]));
        DebugWindow.Initialize();

        OffsetIndex += Entry->GetSize();
        Entry = Entry->GetNextEntry();
    }
}

void FKLDebugImGuiFeatureContainer::Shutdown()
{
    DestroyFeatures();
}

void FKLDebugImGuiFeatureContainer::DestroyFeatures()
{
    mFeaturesOffset.Empty();
    mFeaturesPool.Empty();
}
