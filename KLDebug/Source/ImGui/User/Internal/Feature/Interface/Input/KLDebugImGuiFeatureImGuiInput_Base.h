// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureImGuiInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureImGuiInput_Base(const UWorld& _World, bool& _IsWindowOpen);
    virtual ~FKLDebugImGuiFeatureImGuiInput_Base() = default;

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD bool& GetIsWindowOpen() const;

private:
    const UWorld& mWorld;
    bool& mIsWindowOpen;
};

inline const UWorld& FKLDebugImGuiFeatureImGuiInput_Base::GetWorld() const
{
    return mWorld;
}

inline bool& FKLDebugImGuiFeatureImGuiInput_Base::GetIsWindowOpen() const
{
    return mIsWindowOpen;
}
