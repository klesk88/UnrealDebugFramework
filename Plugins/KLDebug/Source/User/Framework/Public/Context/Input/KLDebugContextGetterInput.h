// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Engine/EngineBaseTypes.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

/*
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugContextGetterInput : public FNoncopyable
{
public:
    explicit FKLDebugContextGetterInput(const UWorld& _World, const ENetMode _CurrentNetMode);
    virtual ~FKLDebugContextGetterInput() = default;

    [[nodiscard]] const UWorld& GetWorld() const;
    [[nodiscard]] ENetMode GetCurrentNetMode() const;

private:
    const UWorld& mWorld;
    ENetMode mCurrentNetMode = ENetMode::NM_MAX;
};

inline const UWorld& FKLDebugContextGetterInput::GetWorld() const
{
    return mWorld;
}

inline ENetMode FKLDebugContextGetterInput::GetCurrentNetMode() const
{
    return mCurrentNetMode;
}
