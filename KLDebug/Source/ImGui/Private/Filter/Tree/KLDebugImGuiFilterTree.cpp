#include "Filter/Tree/KLDebugImGuiFilterTree.h"

#include "Feature/Container/Iterators/KLDebugImGuiFeaturesIterator.h"
#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"
#include "Filter/Interface/KLDebugImGuiFilterInterface.h"
#include "Filter/Manager/KLDebugImGuiFilterManager.h"
#include "Filter/Manager/KLDebugImGuiFilterManagerEntryBase.h"
#include "Filter/Tree/KLDebugImGuiFilterNodesStackData.h"
#include "Filter/Tree/KLDebugImGuiTreeSortedFeatures.h"
#include "TreeBuilder/KLDebugImGuiTreeBuilderHelpers.h"

// debug utils module
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Containers/UnrealString.h"
#include "Math/NumericLimits.h"

void FKLDebugImGuiFilterTree::Init(const int32 _FeaturesCount, FKLDebugImGuiFeaturesIterator& _Iterator)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiFilterTree_Init);

    checkf(_FeaturesCount < TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max(), TEXT("too many features"));

    GenerateFilters();
    if (mFiltersPool.IsEmpty())
    {
        return;
    }

    GenerateTree(_FeaturesCount, _Iterator);
}

void FKLDebugImGuiFilterTree::GatherFeatures(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const
{
    _OutFeaturesIndexes.Reserve(mFeaturesWithoutFilters.Num() + mFilterFeaturesIndexes.Num());
    for (const KL::Debug::ImGui::Features::Types::FeatureIndex Index : mFeaturesWithoutFilters)
    {
        _OutFeaturesIndexes.Emplace(Index);
    }

    if (mTreeNodes.IsEmpty())
    {
        UE_LOG(LogKL_Debug, Error, TEXT("No filters"));
        return;
    }

    auto KeepTraversingTreeLambda = [this, &_Obj](const FKLDebugImGuiFilterTreeNode& _TreeNode) -> bool {
        const uint16 FilterIndex = _TreeNode.GetFilterIndex();
        checkf(mFiltersPool.IsValidIndex(FilterIndex), TEXT("index must be valid"));
        const IKLDebugImGuiFilterInterface& FilterInterface = *reinterpret_cast<const IKLDebugImGuiFilterInterface*>(&mFiltersPool[FilterIndex]);
        const bool                          IsValid         = FilterInterface.IsObjectValid(_Obj);
        return IsValid;
    };

    auto EvaluateNodeLambda = [this, &_OutFeaturesIndexes](const FKLDebugImGuiFilterTreeNode& _TreeNode) -> void {
        const TOptional<uint16> NodeDataIndex = _TreeNode.GetNodeDataIndex();
        if (!NodeDataIndex.IsSet())
        {
            return;
        }

        const FKLDebugImGuiFilterTreeNodeData&                NodeData        = mTreeNodesData[NodeDataIndex.GetValue()];
        const KL::Debug::ImGui::Features::Types::FeatureIndex StartFeatureIdx = NodeData.GetStartDataOffset();
        const KL::Debug::ImGui::Features::Types::FeatureIndex EndFeatureIdx   = NodeData.GetEndDataOffset();

        check(StartFeatureIdx != TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max() && EndFeatureIdx != TNumericLimits<KL::Debug::ImGui::Features::Types::FeatureIndex>::Max());

        for (int32 i = static_cast<int32>(StartFeatureIdx); i < static_cast<int32>(EndFeatureIdx); ++i)
        {
            _OutFeaturesIndexes.Emplace(mFilterFeaturesIndexes[i]);
        }
    };

    KL::Debug::ImGuiTreeBuilder::Helpers::PreoderTraversal(mTreeNodes, KeepTraversingTreeLambda, EvaluateNodeLambda);

    _OutFeaturesIndexes.Shrink();
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

void FKLDebugImGuiFilterTree::GenerateTree(const int32 _FeaturesCount, FKLDebugImGuiFeaturesIterator& _Iterator)
{
    TArray<FKLDebugImGuiTreeSortedFeatures> SortedFeatures;
    SortedFeatures.Reserve(_FeaturesCount);
    mFilterFeaturesIndexes.Reserve(_FeaturesCount);

    SortFeatures(_Iterator, SortedFeatures);
    GenerateTree(SortedFeatures);
}

void FKLDebugImGuiFilterTree::SortFeatures(FKLDebugImGuiFeaturesIterator& _Iterator, TArray<FKLDebugImGuiTreeSortedFeatures>& _OutSortedFeatures)
{
    mFeaturesWithoutFilters.Reserve(100);

    TArray<FName> FiltersNames;
    FiltersNames.Reserve(mFiltersOffset.Num());

    FString FilterString;
    FilterString.Reserve(mFilterStringSize);

    int32 Index = 0;
    for (; _Iterator; ++_Iterator)
    {
        const IKLDebugImGuiFeatureInterface_SelectableObject& Feature = _Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_SelectableObject>();
        FiltersNames.Reset();
        Feature.GetFilterPath(FiltersNames);
        if (FiltersNames.IsEmpty())
        {
            mFeaturesWithoutFilters.Emplace(_Iterator.GetFeatureDataIndex());
            continue;
        }

        checkf(!FiltersNames.IsEmpty(), TEXT("first element must be root do not ovveride it and we expect some valid filters"));
        const FName FullFilterPath = KL::Debug::ImGuiTreeBuilder::Helpers::GetFullPathName(FiltersNames, FilterString);
        _OutSortedFeatures.Emplace_GetRef(_Iterator.GetFeatureDataIndex(), FullFilterPath, FiltersNames, Feature);
    }

    mFeaturesWithoutFilters.Shrink();

    _OutSortedFeatures.Sort([](const FKLDebugImGuiTreeSortedFeatures& _Left, const FKLDebugImGuiTreeSortedFeatures& _Right) {
        return _Left.GetFullPath().LexicalLess(_Right.GetFullPath());
    });
}

void FKLDebugImGuiFilterTree::GenerateTree(const TArray<FKLDebugImGuiTreeSortedFeatures>& _SortedFeatures)
{
    auto IsSameDataLambda = [this](const FKLDebugImGuiTreeBuilderStackData& _LastNodeInStack, const FKLDebugImGuiTreeSortedFeatures& _Feature) -> bool {
        bool IsSameFilter = false;
        if (_LastNodeInStack.GetFullPath() == _Feature.GetFullPath())
        {
            mFilterFeaturesIndexes.Emplace(_Feature.GetFeatureDataIndex());
            IsSameFilter = true;
        }

        const uint16                  TreeNodeIndex = _LastNodeInStack.GetTreeNodeIndex();
        FKLDebugImGuiTreeBuilderNode& TreeNode      = mTreeNodes[TreeNodeIndex];
        const TOptional<uint16>       NodeDataIndex = TreeNode.GetNodeDataIndex();

        if (NodeDataIndex.IsSet())
        {
            FKLDebugImGuiFilterTreeNodeData& TreNodeData = mTreeNodesData[NodeDataIndex.GetValue()];
            TreNodeData.SetEndDataOffset(mFilterFeaturesIndexes.Num());
        }

        return IsSameFilter;
    };

    auto AllocateTreeNode = [this](const FName& _FilterToken) -> FKLDebugImGuiFilterTreeNode& {
        const uint16 FilterIdx = GetFilterIndexFromID(_FilterToken, NAME_None);
        return mTreeNodes.Emplace_GetRef(FilterIdx);
    };

    auto AllocateTreeNodeLeaf = [this](const FName& _FilterToken, const FKLDebugImGuiTreeSortedFeatures& _SortedFeature) -> FKLDebugImGuiFilterTreeNode& {
        const IKLDebugImGuiFeatureInterface_SelectableObject& Feature   = _SortedFeature.GetFeature();
        const uint16                                          FilterIdx = GetFilterIndexFromID(_FilterToken, Feature.StaticItemType());

        mTreeNodesData.Emplace(mFilterFeaturesIndexes.Num());
        mFilterFeaturesIndexes.Emplace(_SortedFeature.GetFeatureDataIndex());
        return mTreeNodes.Emplace_GetRef(mTreeNodesData.Num() - 1, FilterIdx);
    };

    KL::Debug::ImGuiTreeBuilder::Helpers::GenerateTree(_SortedFeatures, mTreeNodes, IsSameDataLambda, AllocateTreeNode, AllocateTreeNodeLeaf);

    mTreeNodes.Shrink();
    mTreeNodesData.Shrink();
}

KL::Debug::ImGui::Features::Types::FilterIndex FKLDebugImGuiFilterTree::GetFilterIndexFromID(const FName& _FilterID, const FName& _FeatureType) const
{
    for (const uint32 FilterIdx : mFiltersOffset)
    {
        const IKLDebugImGuiFilterInterface* FilterInterface = reinterpret_cast<const IKLDebugImGuiFilterInterface*>(&mFiltersPool[FilterIdx]);
        if (FilterInterface->GetFilterID() == _FilterID)
        {
            checkf(FilterIdx < TNumericLimits<uint16>::Max(), TEXT("too many filters"));
            return static_cast<KL::Debug::ImGui::Features::Types::FilterIndex>(FilterIdx);
        }
    }

    UE_LOG(LogKL_Debug, Error, TEXT("filters not found for feature [%s]"), *_FeatureType.ToString())
    return TNumericLimits<KL::Debug::ImGui::Features::Types::FilterIndex>::Max();
}
