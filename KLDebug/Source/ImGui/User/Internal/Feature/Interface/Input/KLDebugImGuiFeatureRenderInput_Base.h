#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureRenderInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureRenderInput_Base(const UWorld& _World);
    virtual ~FKLDebugImGuiFeatureRenderInput_Base() = default;

    UE_NODISCARD const UWorld& GetWorld() const;

private:
    const UWorld& mWorld;
};

inline const UWorld& FKLDebugImGuiFeatureRenderInput_Base::GetWorld() const
{
    return mWorld;
}
