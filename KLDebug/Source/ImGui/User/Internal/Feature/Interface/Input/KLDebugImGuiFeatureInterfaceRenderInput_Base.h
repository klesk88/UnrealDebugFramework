#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureInterfaceRenderInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureInterfaceRenderInput_Base(const UWorld& _World);
    virtual ~FKLDebugImGuiFeatureInterfaceRenderInput_Base() = default;

    UE_NODISCARD const UWorld& GetWorld() const;

private:
    const UWorld& mWorld;
};

inline const UWorld& FKLDebugImGuiFeatureInterfaceRenderInput_Base::GetWorld() const
{
    return mWorld;
}
