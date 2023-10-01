#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"

#include "Feature/Manager/KLDebugImGuiFeatureManager.h"
#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

//modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "Utils/Public/KLDebugLog.h"

const IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeatureContainerBase::GetFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const
{
    checkf(mFeaturesData.IsValidIndex(_FeatureIndex), TEXT("mFeaturesData Out of bounds"));
    const FKLDebugImGuiFeatureData& FeatureData = mFeaturesData[_FeatureIndex];
    checkf(mFeaturesPool.IsValidIndex(FeatureData.GetFeatureOffset()), TEXT("mFeaturesPool Out of bounds"));
    return *reinterpret_cast<const IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureData.GetFeatureOffset()]);
}

IKLDebugImGuiFeatureInterfaceBase& FKLDebugImGuiFeatureContainerBase::GetFeatureMutable(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
{
    checkf(mFeaturesData.IsValidIndex(_FeatureIndex), TEXT("mFeaturesData Out of bounds"));
    const FKLDebugImGuiFeatureData& FeatureData = mFeaturesData[_FeatureIndex];
    checkf(mFeaturesPool.IsValidIndex(FeatureData.GetFeatureOffset()), TEXT("mFeaturesPool Out of bounds"));
    return *reinterpret_cast<IKLDebugImGuiFeatureInterfaceBase*>(&mFeaturesPool[FeatureData.GetFeatureOffset()]);
}

void FKLDebugImGuiFeatureContainerBase::AllocateNewEntry(const FKLDebugImGuiFeatureManagerEntryBase& _Entry, const KL::Debug::ImGui::Features::Types::FeatureOffset _OffsetIndex, TArray<FString>& _PathString)
{
    IKLDebugImGuiFeatureInterfaceBase& DebugWindow    = _Entry.AllocateInPlace(static_cast<void*>(&mFeaturesPool[_OffsetIndex]));
    FKLDebugImGuiFeatureData&          NewFeatureData = mFeaturesData.Emplace_GetRef(_OffsetIndex);
    NewFeatureData.Init(DebugWindow, _PathString);

    DebugWindow.Initialize();
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

    const IKLDebugImGuiFeatureInterfaceBase& Feature = GetFeature(_FeatureIndex);
    return TOptional<FName>(Feature.GetFeatureNameID());
}
