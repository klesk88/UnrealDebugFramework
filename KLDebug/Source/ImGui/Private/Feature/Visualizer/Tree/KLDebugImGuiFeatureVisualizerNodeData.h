#pragma once

// engine
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerNodeData final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerNodeData(const FName& _Name);

    UE_NODISCARD const FString& GetImGuiNodeString() const;

    UE_NODISCARD bool& GetIsOpen();
    UE_NODISCARD bool& GetIsSelected();

private:
    FString mNameAsString;
    bool    mIsOpen     = false;
    bool    mIsSelected = false;
};

inline const FString& FKLDebugImGuiFeatureVisualizerNodeData::GetImGuiNodeString() const
{
    return mNameAsString;
}

inline bool& FKLDebugImGuiFeatureVisualizerNodeData::GetIsOpen()
{
    return mIsOpen;
}

inline bool& FKLDebugImGuiFeatureVisualizerNodeData::GetIsSelected()
{
    return mIsSelected;
}