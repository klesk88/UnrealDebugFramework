// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Features/Engine/Picker/KLDebugUtilsPicker.h"

// modules
#include "User/Framework/Public/Feature/Interface/Unique/KLDebugFeatureInterface_Unique.h"

// engine
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AActor;
class UWorld;

class KLDEBUGEXAMPLES_API FKLDebugFeatureEngine_Picker : public IKLDebugFeatureInterface_Unique
{
public:
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureEngine_Picker, IKLDebugFeatureInterface_Unique)

public:
    FKLDebugFeatureEngine_Picker();

private:
    // IKLDebugImGuiFeatureInterface_EngineSubsystem
    UE_NODISCARD bool DoesSupportWorld(const UWorld& _World) const final;
    UE_NODISCARD const FName& GetImGuiPath() const;
    UE_NODISCARD const FString& GetWindowName() const final;
    void DrawImGuiChild(FKLDebugFeatureImGuiInput_Unique& _Input) final;
    // IKLDebugImGuiFeatureInterface_EngineSubsystem

    void HandlePickerButton(const UWorld& _World);
    void ImguiDrawPickedCharacterInfo() const;

private:
    FKLDebugUtilsPicker mPicker;
    TWeakObjectPtr<AActor> mPickedActor;
};

inline bool FKLDebugFeatureEngine_Picker::DoesSupportWorld([[maybe_unused]] const UWorld& _World) const
{
    return true;
}