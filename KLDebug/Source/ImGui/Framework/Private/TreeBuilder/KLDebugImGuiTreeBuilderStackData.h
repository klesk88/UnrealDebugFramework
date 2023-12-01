// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiTreeBuilderStackData final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiTreeBuilderStackData(const FName& _FullPath, const FName& _NodeName, const int32 _TreeNodeIdx);

    UE_NODISCARD const FName& GetFullPath() const;
    UE_NODISCARD const FName& GetNodeName() const;
    UE_NODISCARD uint16 GetTreeNodeIndex() const;

private:
    FName mFullPath;
    FName mNodeName;
    uint16 mTreeNode;
};

inline const FName& FKLDebugImGuiTreeBuilderStackData::GetFullPath() const
{
    return mFullPath;
}

inline const FName& FKLDebugImGuiTreeBuilderStackData::GetNodeName() const
{
    return mNodeName;
}

inline uint16 FKLDebugImGuiTreeBuilderStackData::GetTreeNodeIndex() const
{
    return mTreeNode;
}