// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Interface/Input/KLDebugFeatureDrawCanvasInput_Base.h"

class UObject;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureDrawCanvasInput_Selectable final : public FKLDebugFeatureDrawCanvasInput_Base
{
public:
    explicit FKLDebugFeatureDrawCanvasInput_Selectable(const UObject& _Owner, UCanvas& _Canvas, UFont& _Font, IKLDebugContextInterface* _Context);

    [[nodiscard]] const UObject& GetOwnerObject() const;

private:
    const UObject& mOwnerObject;
};

inline const UObject& FKLDebugFeatureDrawCanvasInput_Selectable::GetOwnerObject() const
{
    return mOwnerObject;
}
