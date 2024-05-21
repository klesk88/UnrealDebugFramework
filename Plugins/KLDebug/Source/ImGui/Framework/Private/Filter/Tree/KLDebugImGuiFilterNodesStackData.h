// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

// #pragma once
//
// #include "TreeBuilder/KLDebugImGuiTreeBuilderStackData.h"
//
//// engine
// #include "Containers/Array.h"
// #include "CoreMinimal.h"
// #include "Math/NumericLimits.h"
// #include "Templates/UnrealTemplate.h"
// #include "UObject/NameTypes.h"
//
// class FKLDebugImGuiFilterTreeNode;
//
// class FKLDebugImGuiFilterNodesStackData final : public FNoncopyable
//{
// public:
//     explicit FKLDebugImGuiFilterNodesStackData(const FName& _FilterTag, const FName& _FullFilter, const int32 _TreeNodeIndex);
//
//     [[nodiscard]] const FName&                 GetFilterTag() const;
//     [[nodiscard]] const FName&                 GetFullFilter() const;
//     [[nodiscard]] FKLDebugImGuiFilterTreeNode& GetTreeNode(TArray<FKLDebugImGuiFilterTreeNode>& _TreeNodes) const;
//
// private:
//     FName  mFilterTag;
//     FName  mFullFilter;
//     uint16 mTreeNodeIndex = TNumericLimits<uint16>::Max();
// };
//
// inline const FName& FKLDebugImGuiFilterNodesStackData::GetFilterTag() const
//{
//     return mFilterTag;
// }
//
// inline const FName& FKLDebugImGuiFilterNodesStackData::GetFullFilter() const
//{
//     return mFullFilter;
// }
//
// inline FKLDebugImGuiFilterTreeNode& FKLDebugImGuiFilterNodesStackData::GetTreeNode(TArray<FKLDebugImGuiFilterTreeNode>& _TreeNodes) const
//{
//     check(mTreeNodeIndex != TNumericLimits<uint16>::Max());
//     return _TreeNodes[mTreeNodeIndex];
// }