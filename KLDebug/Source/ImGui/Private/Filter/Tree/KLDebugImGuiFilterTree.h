#pragma once

#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Filter/Tree/KLDebugImGuiFilterTreeNode.h"
#include "Filter/Tree/KLDebugImGuiFilterTreeNodeData.h"

// engine
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class FKLDebugImGuiFeaturesIterator;
class FKLDebugImGuiFilterNodesStackData;
class FKLDebugImGuiTreeSortedFeatures;
class FString;
class UObject;

/*
 * The tree is a flat array of a non binary unbalanced tree of Gameplay tags which composed all the features we have.
 * if for example we have only one feature implemented called KL.Debug.AI.Cover and the root of the tree we are give is KL.Debug,
 * the tree will be represented by 2 entries one for KL.Debug.AI and one for KL.Debug.AI.Cover
 */
class KLDEBUGIMGUI_API FKLDebugImGuiFilterTree final : public FNoncopyable
{
public:
    void Init(const int32 _FeaturesCount, FKLDebugImGuiFeaturesIterator& _Iterator);
    void GatherFeatures(const UObject& _Obj, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const;

    UE_NODISCARD bool HasFilters() const;

private:
    void         GenerateFilters();
    void         GenerateTree(const int32 _FeaturesCount, FKLDebugImGuiFeaturesIterator& _Iterator);
    void         SortFeatures(FKLDebugImGuiFeaturesIterator& _Iterator, TArray<FKLDebugImGuiTreeSortedFeatures>& _OutSortedFeature);
    void         GenerateTree(const TArray<FKLDebugImGuiTreeSortedFeatures>& _SortedFeatures);
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FilterIndex GetFilterIndexFromID(const FName& _FilterID, const FName& _FeatureType) const;

private:
    // offset between features so that we can retrieve them correctly from the pool. Each entry is the
    // start address index of a new feature inside mFeaturesPool.
    // This array is sorted based on the game play tags used
    TArray<KL::Debug::ImGui::Features::Types::FilterIndex> mFiltersOffset;
    // pool of features. This is a byte array and all features are allocated inside of it
    // in this way they are all packed in memory close together
    TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue> mFiltersPool;
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>    mFilterFeaturesIndexes;

    TArray<FKLDebugImGuiFilterTreeNode>     mTreeNodes;
    TArray<FKLDebugImGuiFilterTreeNodeData> mTreeNodesData;

    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> mFeaturesWithoutFilters;

    static constexpr int32 mFilterStringSize = 300;

#if WITH_AUTOMATION_TESTS
public:
    void               TestInitWithExternalData(TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue>&& _FilterPool, TArray<KL::Debug::ImGui::Features::Types::FilterIndex>&& _FilterOffsets);
    void               TestGenerateTree(const int32 _FeaturesCount, FKLDebugImGuiFeaturesIterator& _Iterator);
    UE_NODISCARD const TArray<FKLDebugImGuiFilterTreeNode>& TestGetTreeNodes() const;
    UE_NODISCARD const TArray<FKLDebugImGuiFilterTreeNodeData>& TestGetTreeNodesData() const;
    UE_NODISCARD const TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue>& TestGetFilterPool() const;
#endif
};

inline bool FKLDebugImGuiFilterTree::HasFilters() const
{
    return !mTreeNodes.IsEmpty();
}

#if WITH_AUTOMATION_TESTS

inline void FKLDebugImGuiFilterTree::TestInitWithExternalData(TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue>&& _FilterPool, TArray<KL::Debug::ImGui::Features::Types::FilterIndex>&& _FilterOffsets)
{
    mFiltersOffset = MoveTemp(_FilterOffsets);
    mFiltersPool   = MoveTemp(_FilterPool);
}

inline void FKLDebugImGuiFilterTree::TestGenerateTree(const int32 _FeaturesCount, FKLDebugImGuiFeaturesIterator& _Iterator)
{
    GenerateTree(_FeaturesCount, _Iterator);
}

inline const TArray<FKLDebugImGuiFilterTreeNode>& FKLDebugImGuiFilterTree::TestGetTreeNodes() const
{
    return mTreeNodes;
}

inline const TArray<FKLDebugImGuiFilterTreeNodeData>& FKLDebugImGuiFilterTree::TestGetTreeNodesData() const
{
    return mTreeNodesData;
}

inline const TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue>& FKLDebugImGuiFilterTree::TestGetFilterPool() const
{
    return mFiltersPool;
}

#endif