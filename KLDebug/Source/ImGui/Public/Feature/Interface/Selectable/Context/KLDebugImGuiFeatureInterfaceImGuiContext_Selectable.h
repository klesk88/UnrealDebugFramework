#pragma once

#include "Feature/Interface/Private/Context/KLDebugImGuiFeatureInterfaceImGuiContext_Base.h"

class UObject;
class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureInterfaceImGuiContext_Selectable final : public FKLDebugImGuiFeatureInterfaceImGuiContext_Base
{
public:
    explicit FKLDebugImGuiFeatureInterfaceImGuiContext_Selectable(const UWorld& _World, bool& _IsWindowOpen, UObject& _Object);

    UE_NODISCARD const UObject& GetObject() const;
    UE_NODISCARD UObject& GetObjectMutable() const;

private:
    UObject& mObject;
};

inline const UObject& FKLDebugImGuiFeatureInterfaceImGuiContext_Selectable::GetObject() const
{
    return GetObjectMutable();
}

inline UObject& FKLDebugImGuiFeatureInterfaceImGuiContext_Selectable::GetObjectMutable() const
{
    return mObject;
}