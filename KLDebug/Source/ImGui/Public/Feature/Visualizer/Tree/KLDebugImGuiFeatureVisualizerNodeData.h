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

    UE_NODISCARD bool GetIsSelected() const;

    void ToogleIsSelected();

private:
    FString mNameAsString;
    bool    mIsSelected = false;
};

inline const FString& FKLDebugImGuiFeatureVisualizerNodeData::GetImGuiNodeString() const
{
    return mNameAsString;
}

inline bool FKLDebugImGuiFeatureVisualizerNodeData::GetIsSelected() const
{
    return mIsSelected;
}

inline void FKLDebugImGuiFeatureVisualizerNodeData::ToogleIsSelected()
{
    mIsSelected = !mIsSelected;
}
