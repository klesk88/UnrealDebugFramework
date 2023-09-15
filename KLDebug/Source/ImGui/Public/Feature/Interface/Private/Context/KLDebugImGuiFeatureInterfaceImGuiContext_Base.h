#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureInterfaceImGuiContext_Base : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureInterfaceImGuiContext_Base(const UWorld& _World, bool& _IsWindowOpen);
    virtual ~FKLDebugImGuiFeatureInterfaceImGuiContext_Base() = default;

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD bool& GetIsWindowOpen() const;

private:
    const UWorld& mWorld;
    bool& mIsWindowOpen;
};

inline const UWorld& FKLDebugImGuiFeatureInterfaceImGuiContext_Base::GetWorld() const
{
    return mWorld;
}

inline bool& FKLDebugImGuiFeatureInterfaceImGuiContext_Base::GetIsWindowOpen() const
{
    return mIsWindowOpen;
}
