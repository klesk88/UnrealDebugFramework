#include "Feature/Container/KLDebugImGuiFeatureContainer.h"

#include "Feature/Interface/KLDebugImGuiFeatureInterface.h"
#include "Feature/Manager/KLDebugImGuiFeatureManager.h"
#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

//debug core module
#include "Core/Public/KLDebugLog.h"

//engine
#include "GameplayTagContainer.h"

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
    SortFeatures();
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
        
#if DO_ENSURE
        const FGameplayTag& Tag = DebugWindow.GetTag();
        if (ensureMsgf(Tag != FGameplayTag::EmptyTag, TEXT("one debug feature has no tag set. Please add a valid tag")))
        {
            const bool ContainsString = Tag.ToString().Contains(TEXT("KL.Debug"));
            UE_CLOG(!ContainsString, LogKL_Debug, Error, TEXT("The debug feature gameplay tag do not start from KL.Debug. Please use this path as root"));
        }
#endif
        
        DebugWindow.Initialize();

        OffsetIndex += Entry->GetSize();
        Entry = Entry->GetNextEntry();
    }
}

void FKLDebugImGuiFeatureContainer::SortFeatures()
{
    mFeaturesOffset.Sort([this](const uint32 _Left, const uint32 _Right) {
        const IKLDebugImGuiFeatureInterface& LeftFeature = GetFeature(_Left);
        const IKLDebugImGuiFeatureInterface& RightFeature = GetFeature(_Right);

        const FGameplayTag& LeftGT = LeftFeature.GetTag();
        const FGameplayTag& RightGT = RightFeature.GetTag();
       
        return LeftGT.GetTagName().LexicalLess(RightGT.GetTagName());
    });
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
