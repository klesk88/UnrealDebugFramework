// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"

class FKLDebugImGuiFeatureImGuiInput_Unique;
class UObject;

class KLDEBUGIMGUIDEBUG_API FKLDebugImguiFeature_ClientSubsystem final : public IKLDebugImGuiFeatureInterface_Unique
{
public:
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugImguiFeature_ClientSubsystem, IKLDebugImGuiFeatureInterface_Unique)

private:
    // IKLDebugImGuiFeatureInterface_EngineSubsystem
    UE_NODISCARD bool DoesSupportObject(const UObject& _Object) const final;
    UE_NODISCARD const FName& GetImGuiPath() const;
    UE_NODISCARD const FString& GetWindowName() const final;
    void DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Unique& _Input) final;
    // IKLDebugImGuiFeatureInterface_EngineSubsystem
};
