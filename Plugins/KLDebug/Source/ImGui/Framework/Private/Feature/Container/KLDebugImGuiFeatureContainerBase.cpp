// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"

// modules
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceBase.h"
#include "User/Framework/Internal/Feature/Manager/KLDebugFeatureManager.h"
#include "User/Framework/Internal/Feature/Manager/KLDebugFeatureManagerEntryBase.h"
#include "Utils/Public/KLDebugLog.h"

const IKLDebugFeatureInterfaceBase& FKLDebugImGuiFeatureContainerBase::GetFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const
{
    checkf(mFeaturesData.IsValidIndex(_FeatureIndex), TEXT("mFeaturesData Out of bounds"));
    const FKLDebugImGuiFeatureData& FeatureData = mFeaturesData[_FeatureIndex];
    checkf(mFeaturesPool.IsValidIndex(FeatureData.GetFeatureOffset()), TEXT("mFeaturesPool Out of bounds"));
    return *reinterpret_cast<const IKLDebugFeatureInterfaceBase*>(&mFeaturesPool[FeatureData.GetFeatureOffset()]);
}

IKLDebugFeatureInterfaceBase& FKLDebugImGuiFeatureContainerBase::GetFeatureMutable(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
{
    checkf(mFeaturesData.IsValidIndex(_FeatureIndex), TEXT("mFeaturesData Out of bounds"));
    const FKLDebugImGuiFeatureData& FeatureData = mFeaturesData[_FeatureIndex];
    checkf(mFeaturesPool.IsValidIndex(FeatureData.GetFeatureOffset()), TEXT("mFeaturesPool Out of bounds"));
    return *reinterpret_cast<IKLDebugFeatureInterfaceBase*>(&mFeaturesPool[FeatureData.GetFeatureOffset()]);
}

IKLDebugFeatureInterfaceBase& FKLDebugImGuiFeatureContainerBase::AllocateNewEntry(const FKLDebugFeatureManagerEntryBase& _Entry, const KL::Debug::ImGui::Features::Types::FeatureOffset _OffsetIndex, TArray<FString>& _PathString)
{
    IKLDebugFeatureInterfaceBase& DebugWindow = _Entry.AllocateInPlace(static_cast<void*>(&mFeaturesPool[_OffsetIndex]));
    FKLDebugImGuiFeatureData& NewFeatureData = mFeaturesData.Emplace_GetRef(_OffsetIndex);
    NewFeatureData.Init(DebugWindow, _PathString);

    DebugWindow.Initialize();
    return DebugWindow;
}

bool FKLDebugImGuiFeatureContainerBase::IsValidFeatureIndex(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const FName& _FeatureNameID) const
{
    const TOptional<FName> FeatureNameID = TryGetFeatureNameID(_FeatureIndex);
    if (FeatureNameID.IsSet())
    {
        return FeatureNameID.GetValue() == _FeatureNameID;
    }
    else
    {
        return false;
    }
}

TOptional<FName> FKLDebugImGuiFeatureContainerBase::TryGetFeatureNameID(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const
{
    if (!mFeaturesData.IsValidIndex(_FeatureIndex))
    {
        return TOptional<FName>();
    }

    const IKLDebugFeatureInterfaceBase& Feature = GetFeature(_FeatureIndex);
    return TOptional<FName>(Feature.GetFeatureNameID());
}
