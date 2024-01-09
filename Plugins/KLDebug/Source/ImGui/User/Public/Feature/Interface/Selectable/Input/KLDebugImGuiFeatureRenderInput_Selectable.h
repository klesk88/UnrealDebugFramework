// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "Feature/Interface/Input/KLDebugImGuiFeatureRenderInput_Base.h"

class UWorld;
class UObject;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureRenderInput_Selectable final : public FKLDebugImGuiFeatureRenderInput_Base
{
public:
    explicit FKLDebugImGuiFeatureRenderInput_Selectable(const UWorld& _World, const UObject& _Object, FKLDebugImGuiFeatureContext_Base* _Context);

    UE_NODISCARD const UObject& GetObject() const;

private:
    const UObject& mObject;
};

inline const UObject& FKLDebugImGuiFeatureRenderInput_Selectable::GetObject() const
{
    return mObject;
}
