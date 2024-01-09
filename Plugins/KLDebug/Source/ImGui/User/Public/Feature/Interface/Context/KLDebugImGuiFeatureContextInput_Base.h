// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Templates/UnrealTemplate.h"

/*
 */
class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureContextInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureContextInput_Base(const ENetMode _CurrentNetMode);

    UE_NODISCARD ENetMode GetCurrentNetMode() const;
    UE_NODISCARD bool HasAuthorityOnObject() const;

protected:
    bool mHasAutorithyOnObject = true;

private:
    ENetMode mCurrentNetMode = ENetMode::NM_MAX;
};

inline ENetMode FKLDebugImGuiFeatureContextInput_Base::GetCurrentNetMode() const
{
    return mCurrentNetMode;
}

inline bool FKLDebugImGuiFeatureContextInput_Base::HasAuthorityOnObject() const
{
    return mHasAutorithyOnObject;
}
