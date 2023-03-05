#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Manager/KLDebugImGuiFeatureManager.h"
#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

// debug utils module
#include "Utils/Public/KLDebugLog.h"

IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeatureContainerBase::GetFeatureMutable(const KL::Debug::Features::Types::FeatureIndex _Offset)
{
    checkf(_Offset < static_cast<uint32>(mFeaturesPool.Num()), TEXT("Out of bounds"));
    return *reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[_Offset]);
}

const IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeatureContainerBase::GetFeature(const KL::Debug::Features::Types::FeatureIndex _Offset) const
{
    checkf(_Offset < static_cast<uint32>(mFeaturesPool.Num()), TEXT("Out of bounds"));
    return *reinterpret_cast<const IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[_Offset]);
}

void FKLDebugImGuiFeatureContainerBase::AllocateNewEntry(const FKLDebugImGuiFeatureManagerEntryBase& _Entry, const KL::Debug::Features::Types::FeatureIndex _OffsetIndex)
{
    IKLDebugImGuiFeatureInterfaceBase& DebugWindow = _Entry.AllocateInPlace(static_cast<void*>(&mFeaturesPool[_OffsetIndex]));
    mFeaturesOffset.Emplace(_OffsetIndex);
    DebugWindow.Initialize();
}
