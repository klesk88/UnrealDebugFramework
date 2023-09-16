#pragma once

#include "Feature/Interface/Private/Input/KLDebugImGuiFeatureInterfaceRenderInput_Base.h"

class UWorld;
class UObject;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureInterfaceRenderInput_Selectable final : public FKLDebugImGuiFeatureInterfaceRenderInput_Base
{
public:
    explicit FKLDebugImGuiFeatureInterfaceRenderInput_Selectable(const UWorld& _World, const UObject& _Object);

    UE_NODISCARD const UObject& GetObject() const;

private:
    const UObject& mObject;
};

inline const UObject& FKLDebugImGuiFeatureInterfaceRenderInput_Selectable::GetObject() const
{
    return mObject;
}