// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Features/Engine/Picker/KLDebugUtilsPicker.h"

// modules
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"

// engine
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AActor;
class UWorld;

class KLDEBUGEXAMPLES_API FKLDebugFeatureEngine_Picker : public IKLDebugImGuiFeatureInterface_Unique
{
public:
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureEngine_Picker, IKLDebugImGuiFeatureInterface_Unique)

public:
    FKLDebugFeatureEngine_Picker();

private:
    // IKLDebugImGuiFeatureInterface_EngineSubsystem
    UE_NODISCARD bool DoesSupportObject(const UObject& _Object) const final;
    UE_NODISCARD const FName& GetImGuiPath() const;
    UE_NODISCARD const FString& GetWindowName() const final;
    void DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Unique& _Input) final;
    // IKLDebugImGuiFeatureInterface_EngineSubsystem

    void HandlePickerButton(const UWorld& _World);
    void ImguiDrawPickedCharacterInfo() const;

private:
    FKLDebugUtilsPicker mPicker;
    TWeakObjectPtr<AActor> mPickedActor;
};

inline bool FKLDebugFeatureEngine_Picker::DoesSupportObject(const UObject& _Object) const
{
    return true;
}