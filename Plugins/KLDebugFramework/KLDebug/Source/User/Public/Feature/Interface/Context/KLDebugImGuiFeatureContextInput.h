// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Templates/UnrealTemplate.h"

class UObject;

/*
 */
class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureContextInput final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureContextInput(const ENetMode _CurrentNetMode, const UObject& _Object);

    UE_NODISCARD ENetMode GetCurrentNetMode() const;
    UE_NODISCARD bool HasAuthorityOnObject() const;

    // for subsystems interfaces this is the current world
    // for selectable objects interfaces this is the actual object selected
    UE_NODISCARD const UObject& GetObject() const;

private:
    ENetMode mCurrentNetMode = ENetMode::NM_MAX;
    const UObject& mObject;
    bool mHasAutorithyOnObject = true;
};

inline ENetMode FKLDebugImGuiFeatureContextInput::GetCurrentNetMode() const
{
    return mCurrentNetMode;
}

inline const UObject& FKLDebugImGuiFeatureContextInput::GetObject() const
{
    return mObject;
}

inline bool FKLDebugImGuiFeatureContextInput::HasAuthorityOnObject() const
{
    return mHasAutorithyOnObject;
}
