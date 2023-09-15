#pragma once

#include "Feature/Interface/Private/Context/KLDebugImGuiFeatureInterfaceRenderContext_Base.h"

class UWorld;
class UObject;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureInterfaceRenderContext_Selectable final : public FKLDebugImGuiFeatureInterfaceRenderContext_Base
{
public:
    explicit FKLDebugImGuiFeatureInterfaceRenderContext_Selectable(const UWorld& _World, const UObject& _Object);

    UE_NODISCARD const UObject& GetObject() const;

private:
    const UObject& mObject;
};

inline const UObject& FKLDebugImGuiFeatureInterfaceRenderContext_Selectable::GetObject() const
{
    return mObject;
}