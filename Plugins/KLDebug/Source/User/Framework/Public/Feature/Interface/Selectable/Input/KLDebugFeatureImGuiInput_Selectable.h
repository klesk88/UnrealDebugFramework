// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Interface/Input/KLDebugFeatureImGuiInput_Base.h"

class UObject;
class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureImGuiInput_Selectable final : public FKLDebugFeatureImGuiInput_Base
{
public:
    explicit FKLDebugFeatureImGuiInput_Selectable(const UWorld& _World, bool& _IsWindowOpen, IKLDebugContextInterface* _FeatureContext, UObject& _Object);

    [[nodiscard]] const UObject& GetObject() const;
    [[nodiscard]] UObject& GetObjectMutable() const;
    [[nodiscard]] bool HasAuthorityOnObject() const;

private:
    UObject& mObject;
    bool mHasAuthorityOnObject = true;
};

inline const UObject& FKLDebugFeatureImGuiInput_Selectable::GetObject() const
{
    return GetObjectMutable();
}

inline UObject& FKLDebugFeatureImGuiInput_Selectable::GetObjectMutable() const
{
    return mObject;
}

inline bool FKLDebugFeatureImGuiInput_Selectable::HasAuthorityOnObject() const
{
    return mHasAuthorityOnObject;
}