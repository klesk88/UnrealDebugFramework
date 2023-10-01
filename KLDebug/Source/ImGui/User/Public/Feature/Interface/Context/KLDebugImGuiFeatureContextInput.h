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
    //for subsystems interfaces this is the current world
    //for selectable objects interfaces this is the actual object selected
    UE_NODISCARD const UObject& GetObject() const;

private:
    ENetMode mCurrentNetMode = ENetMode::NM_MAX;
    const UObject& mObject;
};

inline ENetMode FKLDebugImGuiFeatureContextInput::GetCurrentNetMode() const
{
    return mCurrentNetMode;
}

inline const UObject& FKLDebugImGuiFeatureContextInput::GetObject() const
{
    return mObject;
}