#pragma once

#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerNodeData.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTreeNode.h"

// engine
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "Containers/Map.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class FKLDebugImGuiFeaturesConstIterator;
class FKLDebugImGuiFeatureVisualizerEntry;
class FKLDebugImGuiVisualizerTreeSortedFeatures;
class FString;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerTree final : public FNoncopyable
{
public:
    void CreateTree(FKLDebugImGuiFeaturesConstIterator& _Iterator);

    void DrawImGuiTree(TArray<FKLDebugImGuiFeatureVisualizerEntry>& _FeaturesIndexesSelected);
    void ClearToogleNodeData(const KL::Debug::ImGui::Features::VisualizerTree::NodeDataID _NodeDataID);

private:
    void GatherAndSortFeatures(FKLDebugImGuiFeaturesConstIterator& _Iterator, TArray<FKLDebugImGuiVisualizerTreeSortedFeatures>& _FeaturesSorted) const;
    void GenerateTree(const TArray<FKLDebugImGuiVisualizerTreeSortedFeatures>& _FeaturesSorted);

private:
    // list of tree nodes
    TArray<FKLDebugImGuiFeatureVisualizerTreeNode> mTreeNodes;
    // list of data per tree node. We store them separately so if we need to regenerate the tree
    // we don't lose the data that we want to keep
    TArray<FKLDebugImGuiFeatureVisualizerNodeData> mNodesData;

#if WITH_AUTOMATION_TESTS
public:
    UE_NODISCARD const TArray<FKLDebugImGuiFeatureVisualizerTreeNode>& TestGetTreeNodes() const;
    UE_NODISCARD const TArray<FKLDebugImGuiFeatureVisualizerNodeData>& TestGetTreeNodesData() const;
#endif
};

#if WITH_AUTOMATION_TESTS

inline const TArray<FKLDebugImGuiFeatureVisualizerTreeNode>& FKLDebugImGuiFeatureVisualizerTree::TestGetTreeNodes() const
{
    return mTreeNodes;
}

inline const TArray<FKLDebugImGuiFeatureVisualizerNodeData>& FKLDebugImGuiFeatureVisualizerTree::TestGetTreeNodesData() const
{
    return mNodesData;
}

#endif