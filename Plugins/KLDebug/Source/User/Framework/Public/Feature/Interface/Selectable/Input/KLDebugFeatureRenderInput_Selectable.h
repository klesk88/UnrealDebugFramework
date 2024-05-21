// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Interface/Input/KLDebugFeatureRenderInput_Base.h"

class UWorld;
class UObject;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureRenderInput_Selectable final : public FKLDebugFeatureRenderInput_Base
{
public:
    explicit FKLDebugFeatureRenderInput_Selectable(const UWorld& _World, const UObject& _Object, IKLDebugContextInterface* _Context);

    [[nodiscard]] const UObject& GetObject() const;

private:
    const UObject& mObject;
};

inline const UObject& FKLDebugFeatureRenderInput_Selectable::GetObject() const
{
    return mObject;
}
