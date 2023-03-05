#include "Filter/Tree/KLDebugImGuiFilterTree.h"

#include "Feature/Container/KLDebugImGuiFeaturesIterator.h"
#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"
#include "Filter/Interface/KLDebugImGuiFilterInterface.h"
#include "Filter/Manager/KLDebugImGuiFilterManager.h"
#include "Filter/Manager/KLDebugImGuiFilterManagerEntryBase.h"
#include "Filter/Tree/KLDebugImGuiFilterNodesStackData.h"
#include "Filter/Tree/KLDebugImGuiTreeSortedFeatures.h"

// debug utils module
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Containers/UnrealString.h"
#include "Math/NumericLimits.h"

void FKLDebugImGuiFilterTree::Init(const int32 _FeaturesCount, FKLDebugImGuiFeaturesIterator& _Iterator)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiFilterTree_Init);

    checkf(_FeaturesCount < TNumericLimits<KL::Debug::Features::Types::FeatureIndex>::Max(), TEXT("too many features"));

    GenerateFilters();
    if (mFiltersPool.IsEmpty())
    {
        return;
    }

    TArray<FKLDebugImGuiTreeSortedFeatures> SortedFeatures;
    SortedFeatures.Reserve(_FeaturesCount);
    mFilterFeaturesIndexes.Reserve(_FeaturesCount);

    SortFeatures(_Iterator, SortedFeatures);
    GenerateTree(SortedFeatures);
}

void FKLDebugImGuiFilterTree::GatherFeatures(const UObject& _Obj, TArray<KL::Debug::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
{
    if (mTreeNodes.IsEmpty())
    {
        UE_LOG(LogKL_Debug, Error, TEXT("No filters"));
        return;
    }

    TArray<const FKLDebugImGuiFilterTreeNode*> NodesStack;
    NodesStack.Reserve(mFiltersOffset.Num());
    NodesStack.Emplace(&mTreeNodes[0]);

    while (!NodesStack.IsEmpty())
    {
        const FKLDebugImGuiFilterTreeNode* TreeNode = NodesStack.Pop();
        TOptional<uint16>                  Index    = TreeNode->GetFilterIndex();
        if (!Index.IsSet())
        {
            UE_LOG(LogKL_Debug, Error, TEXT("No valid filter index"));
            if (TreeNode->GetNextTreeLevelNodeIndex().IsSet())
            {
                NodesStack.Emplace(&mTreeNodes[TreeNode->GetNextTreeLevelNodeIndex().GetValue()]);
            }

            continue;
        }

        const IKLDebugImGuiFilterInterface& FilterInterface = reinterpret_cast<const IKLDebugImGuiFilterInterface&>(mFiltersPool[Index.GetValue()]);
        if (!FilterInterface.IsObjectValid(_Obj))
        {
            if (TreeNode->GetNextTreeLevelNodeIndex().IsSet())
            {
                NodesStack.Emplace(&mTreeNodes[TreeNode->GetNextTreeLevelNodeIndex().GetValue()]);
            }

            continue;
        }

        const uint16 StartFeatureIdx = TreeNode->GetStartDataOffset().IsSet() ? TreeNode->GetStartDataOffset().GetValue() : TNumericLimits<uint16>::Max();
        const uint16 EndFeatureIdx   = TreeNode->GetEndDataOffset().IsSet() ? TreeNode->GetEndDataOffset().GetValue() : TNumericLimits<uint16>::Max();
        if (StartFeatureIdx != TNumericLimits<uint16>::Max() && EndFeatureIdx != TNumericLimits<uint16>::Max())
        {
            for (int32 i = static_cast<int32>(StartFeatureIdx); i < static_cast<int32>(EndFeatureIdx); ++i)
            {
                _OutFeaturesIndexes.Emplace(mFilterFeaturesIndexes[i]);
            }
        }

        for (const uint16 ChildIdx : TreeNode->GetChilds())
        {
            NodesStack.Emplace(&mTreeNodes[ChildIdx]);
        }

        if (TreeNode->GetNextTreeLevelNodeIndex().IsSet())
        {
            NodesStack.Emplace(&mTreeNodes[TreeNode->GetNextTreeLevelNodeIndex().GetValue()]);
        }
    }
}

void FKLDebugImGuiFilterTree::GenerateFilters()
{
    const FKLDebugImGuiFilterManager& FilterManager = FKLDebugImGuiFilterManager::Get();
    mFiltersOffset.Reserve(FilterManager.GetEntryCount());
    mFiltersPool.AddZeroed(FilterManager.GetTotalSizeRequired());

    FKLDebugImGuiFilterManagerEntryBase* Entry       = FilterManager.GetStartEntry();
    uint32                               OffsetIndex = 0;

    TArray<FName> NamesSet;
    NamesSet.Reserve(FilterManager.GetEntryCount());

    while (Entry)
    {
        IKLDebugImGuiFilterInterface& FilterInterface = Entry->AllocateInPlace(static_cast<void*>(&mFiltersPool[OffsetIndex]));
        mFiltersOffset.Emplace(OffsetIndex);

        const FName& ID = FilterInterface.GetFilterID();
        if (NamesSet.Find(ID) != INDEX_NONE)
        {
            UE_LOG(LogKL_Debug, Error, TEXT("The filter [%s] is already present"), *ID.ToString());
            continue;
        }

        NamesSet.Emplace(ID);
        OffsetIndex += Entry->GetSize();
        Entry = Entry->GetNextEntry();
    }
}

void FKLDebugImGuiFilterTree::SortFeatures(FKLDebugImGuiFeaturesIterator& _Iterator, TArray<FKLDebugImGuiTreeSortedFeatures>& _OutSortedFeatures)
{
    TArray<FName> FiltersNames;
    FiltersNames.Reserve(mFiltersOffset.Num());

    FString FilterString;
    FilterString.Reserve(mFilterStringSize);

    int32 Index = 0;
    for (; _Iterator; ++_Iterator)
    {
        const IKLDebugImGuiFeatureInterface_SelectableObject& Feature  = _Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_SelectableObject>();
        FKLDebugImGuiTreeSortedFeatures&                      NewEntry = _OutSortedFeatures.Emplace_GetRef(_Iterator.GetFeatureOffset(), Feature);

        FiltersNames.Reset();
        Feature.GetFilterPath(FiltersNames);

        checkf(!FiltersNames.IsEmpty(), TEXT("first element must be root do not ovveride it and we expect some valid filters"));

        const FName FullFilterPath = GenerateFullFilterPath(FiltersNames, FilterString);
        NewEntry.SetFilters(FiltersNames, FullFilterPath);
    }

    _OutSortedFeatures.Sort([](const FKLDebugImGuiTreeSortedFeatures& _Left, const FKLDebugImGuiTreeSortedFeatures& _Right)
                            { return _Left.GetFullFilter().LexicalLess(_Right.GetFullFilter()); });
}

void FKLDebugImGuiFilterTree::GenerateTree(const TArray<FKLDebugImGuiTreeSortedFeatures>& _SortedFeatures)
{
    static constexpr uint8 MaxTreeDepth  = 40;
    static constexpr uint8 MaxTagsLength = 40;

    FString FilterString;
    FilterString.Reserve(mFilterStringSize);

    TArray<FKLDebugImGuiFilterNodesStackData> NodesStack;
    NodesStack.Reserve(MaxTreeDepth);

    for (const FKLDebugImGuiTreeSortedFeatures& SortedFeature : _SortedFeatures)
    {
        const IKLDebugImGuiFeatureInterface_SelectableObject& Feature = SortedFeature.GetFeature();

        if (!NodesStack.IsEmpty())
        {
            if (NodesStack.Last().GetFullFilter() == SortedFeature.GetFullFilter())
            {
                // same filter so just store the index of the feature and keep going
                mFilterFeaturesIndexes.Emplace(SortedFeature.GetFeatureOffset());
                continue;
            }
            else
            {
                NodesStack.Last().GetTreeNode(mTreeNodes).SetEndDataOffset(mFilterFeaturesIndexes.Num());
            }
        }

        const TArray<FName>& FeatureFiltersSplitted = SortedFeature.GetFilters();

        // go backwards and see if any of the tags is already in the tree
        int32                        StopIndex               = -1;
        FKLDebugImGuiFilterTreeNode* LastNodeInSameTreeLevel = FindMatchingParent(FeatureFiltersSplitted, NodesStack, StopIndex);

        if (StopIndex >= FeatureFiltersSplitted.Num())
        {
            ensureMsgf(false, TEXT("tag already in tree not possible"));
            continue;
        }

        if (LastNodeInSameTreeLevel)
        {
            LastNodeInSameTreeLevel->SetNextTreeLevelNodeIndex(mTreeNodes.Num());
        }

        for (int32 i = StopIndex + 1; i < FeatureFiltersSplitted.Num() - 1; ++i)
        {
            const FName&                  CurrentFilterID = FeatureFiltersSplitted[i];
            const uint16                  FilterIdx       = GetFilterIndexFromID(CurrentFilterID, Feature.GetImGuiPath());
            const TArrayView<const FName> FilterView      = MakeArrayView<const FName>(&FeatureFiltersSplitted[0], i);
            const FName                   FullFilterPath  = GenerateFullFilterPath(FilterView, FilterString);

            mTreeNodes.Emplace(FilterIdx);
            NodesStack.Emplace(FeatureFiltersSplitted[i], FullFilterPath, mTreeNodes.Num() - 1);
        }

        const uint16 FilterIdx = GetFilterIndexFromID(FeatureFiltersSplitted.Last(), Feature.GetImGuiPath());

        if (!NodesStack.IsEmpty())
        {
            NodesStack.Last().GetTreeNode(mTreeNodes).AddChild(mTreeNodes.Num());
        }

        FKLDebugImGuiFilterTreeNode& LastNode = mTreeNodes.Emplace_GetRef(FilterIdx);
        LastNode.SetStartDataOffset(mFilterFeaturesIndexes.Num());
        mFilterFeaturesIndexes.Emplace(SortedFeature.GetFeatureOffset());
        NodesStack.Emplace(FeatureFiltersSplitted.Last(), SortedFeature.GetFullFilter(), mTreeNodes.Num() - 1);
    }

    if (!NodesStack.IsEmpty())
    {
        NodesStack.Last().GetTreeNode(mTreeNodes).SetEndDataOffset(mFilterFeaturesIndexes.Num());
    }
}

FKLDebugImGuiFilterTreeNode* FKLDebugImGuiFilterTree::FindMatchingParent(const TArray<FName>& _FeatureFiltersSplitted, TArray<FKLDebugImGuiFilterNodesStackData>& _TreeNodesStack, int32& _StopIndex)
{
    // go backwards and see if any of the tags is already in the tree

    // node 0 is the current root so we always pass the root at least as taht is the parent
    FKLDebugImGuiFilterTreeNode* LastNodeInSameTreeLevel = nullptr;

    for (int32 i = _FeatureFiltersSplitted.Num() - 1; i >= 0 && !_TreeNodesStack.IsEmpty(); --i)
    {
        const FName& CurrentFilter = _FeatureFiltersSplitted[i];
        if (_TreeNodesStack.Last().GetFilterTag() == CurrentFilter)
        {
            _StopIndex = i;
            break;
        }

        LastNodeInSameTreeLevel = &_TreeNodesStack.Last().GetTreeNode(mTreeNodes);
        // cant use pop as pop returns the last element and we invoke the cpy assignment operator (which we are not interested here)
        _TreeNodesStack.RemoveAt(_TreeNodesStack.Num() - 1, 1, false);
    }

    return LastNodeInSameTreeLevel;
}

uint16 FKLDebugImGuiFilterTree::GetFilterIndexFromID(const FName& _FilterID, const FName& _FeatureName) const
{
    for (const uint32 FilterIdx : mFiltersOffset)
    {
        const IKLDebugImGuiFilterInterface* FilterInterface = reinterpret_cast<const IKLDebugImGuiFilterInterface*>(&mFiltersPool[FilterIdx]);
        if (FilterInterface->GetFilterID() == _FilterID)
        {
            checkf(FilterIdx < TNumericLimits<uint16>::Max(), TEXT("too many filters"));
            return static_cast<uint16>(FilterIdx);
        }
    }

    return TNumericLimits<uint16>::Max();
}

FName FKLDebugImGuiFilterTree::GenerateFullFilterPath(const TArrayView<const FName>& _Filters, FString& _FullFilterString) const
{
    _FullFilterString.Reset();
    for (const FName& Filter : _Filters)
    {
        _FullFilterString.Appendf(TEXT("%s."), *Filter.ToString());
    }

    return FName(_FullFilterString);
}
