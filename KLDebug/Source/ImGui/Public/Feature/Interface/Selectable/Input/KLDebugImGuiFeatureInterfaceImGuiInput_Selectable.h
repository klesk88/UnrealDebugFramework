#pragma once

#include "Feature/Interface/Private/Input/KLDebugImGuiFeatureInterfaceImGuiInput_Base.h"

class FKLDebugImGuiFeatureContext_Base;
class UObject;
class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable final : public FKLDebugImGuiFeatureInterfaceImGuiInput_Base
{
public:
    explicit FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable(const UWorld& _World, bool& _IsWindowOpen, FKLDebugImGuiFeatureContext_Base* _FeatureContext, UObject& _Object);

    UE_NODISCARD const UObject& GetObject() const;
    UE_NODISCARD UObject& GetObjectMutable() const;

private:
    UObject& mObject;
};

inline const UObject& FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable::GetObject() const
{
    return GetObjectMutable();
}

inline UObject& FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable::GetObjectMutable() const
{
    return mObject;
}