#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureInterfaceImGuiInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureInterfaceImGuiInput_Base(const UWorld& _World, bool& _IsWindowOpen);
    virtual ~FKLDebugImGuiFeatureInterfaceImGuiInput_Base() = default;

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD bool& GetIsWindowOpen() const;

private:
    const UWorld& mWorld;
    bool& mIsWindowOpen;
};

inline const UWorld& FKLDebugImGuiFeatureInterfaceImGuiInput_Base::GetWorld() const
{
    return mWorld;
}

inline bool& FKLDebugImGuiFeatureInterfaceImGuiInput_Base::GetIsWindowOpen() const
{
    return mIsWindowOpen;
}
