// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContextInput_Base.h"

class UObject;

/*
 */
class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureContextInput_Selectable final : public FKLDebugImGuiFeatureContextInput_Base
{
public:
    explicit FKLDebugImGuiFeatureContextInput_Selectable(const ENetMode _CurrentNetMode, const UObject& _Object);

    UE_NODISCARD const UObject& GetObject() const;

private:
    const UObject& mObject;
};

inline const UObject& FKLDebugImGuiFeatureContextInput_Selectable::GetObject() const
{
    return mObject;
}
