// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Input/KLDebugImGuiFeatureImGuiInput_Base.h"

class UObject;
class UWorld;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureImGuiInput_Selectable final : public FKLDebugImGuiFeatureImGuiInput_Base
{
public:
    explicit FKLDebugImGuiFeatureImGuiInput_Selectable(const UWorld& _World, bool& _IsWindowOpen, FKLDebugImGuiFeatureContext_Base* _FeatureContext, UObject& _Object);

    UE_NODISCARD const UObject& GetObject() const;
    UE_NODISCARD UObject& GetObjectMutable() const;
    UE_NODISCARD bool HasAuthorityOnObject() const;

private:
    UObject& mObject;
    bool mHasAuthorityOnObject = true;
};

inline const UObject& FKLDebugImGuiFeatureImGuiInput_Selectable::GetObject() const
{
    return GetObjectMutable();
}

inline UObject& FKLDebugImGuiFeatureImGuiInput_Selectable::GetObjectMutable() const
{
    return mObject;
}

inline bool FKLDebugImGuiFeatureImGuiInput_Selectable::HasAuthorityOnObject() const
{
    return mHasAuthorityOnObject;
}