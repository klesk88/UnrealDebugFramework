#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureInterfaceRenderContext_Base : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureInterfaceRenderContext_Base(const UWorld& _World);
    virtual ~FKLDebugImGuiFeatureInterfaceRenderContext_Base() = default;

    UE_NODISCARD const UWorld& GetWorld() const;

private:
    const UWorld& mWorld;
};

inline const UWorld& FKLDebugImGuiFeatureInterfaceRenderContext_Base::GetWorld() const
{
    return mWorld;
}
