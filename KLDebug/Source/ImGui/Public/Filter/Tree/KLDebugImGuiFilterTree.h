#pragma once

#include "Feature/KLDebugFeatureTypes.h"
#include "Filter/Tree/KLDebugImGuiFilterTreeNode.h"

// engine
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "CoreMinimal.h"
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
    void GatherFeatures(const UObject& _Obj, TArray<KL::Debug::Features::Types::FeatureIndex>& _OutFeaturesIndexes) const;
    
    UE_NODISCARD bool HasFilters() const;

private:
    void                                      GenerateFilters();
    void                                      SortFeatures(FKLDebugImGuiFeaturesIterator& _Iterator, TArray<FKLDebugImGuiTreeSortedFeatures>& _OutSortedFeature);
    void                                      GenerateTree(const TArray<FKLDebugImGuiTreeSortedFeatures>& _SortedFeatures);
    UE_NODISCARD FKLDebugImGuiFilterTreeNode* FindMatchingParent(const TArray<FName>& _FeatureFiltersSplitted, TArray<FKLDebugImGuiFilterNodesStackData>& _TreeNodesStack, int32& _StopIndex);
    UE_NODISCARD uint16                       GetFilterIndexFromID(const FName& _FilterID, const FName& _FeatureName) const;
    FName                                     GenerateFullFilterPath(const TArrayView<const FName>& _Filters, FString& _FullFilterString) const;

private:
    // offset between features so that we can retrieve them correctly from the pool. Each entry is the
    // start address index of a new feature inside mFeaturesPool.
    // This array is sorted based on the game play tags used
    TArray<uint32> mFiltersOffset;
    // pool of features. This is a byte array and all features are allocated inside of it
    // in this way they are all packed in memory close together
    TArray<uint8>                                    mFiltersPool;
    TArray<KL::Debug::Features::Types::FeatureIndex> mFilterFeaturesIndexes;
    TArray<FKLDebugImGuiFilterTreeNode>              mTreeNodes;

    static constexpr int32 mFilterStringSize = 300;
};

inline bool FKLDebugImGuiFilterTree::HasFilters() const
{
    return !mTreeNodes.IsEmpty();
}