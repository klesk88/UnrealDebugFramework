#pragma once

#include "Features/Engine/Picker/KLDebugUtilsPicker.h"

// imgui module
#include "ImGui/Public/Feature/Interface/Subsystem/KLDebugImGuiFeatureInterface_EngineSubsystem.h"

//engine
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AActor;
class UWorld;

class KLDEBUGEXAMPLES_API FKLDebugFeatureEngine_Picker : public IKLDebugImGuiFeatureInterface_EngineSubsystem
{
public:
    FKLDebugFeatureEngine_Picker();

private:
    //IKLDebugImGuiFeatureInterface_EngineSubsystem
    UE_NODISCARD const FName& GetImGuiPath() const;
    UE_NODISCARD const FString& GetWindowName() const final;
    void DrawImGuiChild(const UWorld& _World) final;
    //IKLDebugImGuiFeatureInterface_EngineSubsystem

    void HandlePickerButton(const UWorld& _World);
    void ImguiDrawPickedCharacterInfo() const;

private:
    FKLDebugUtilsPicker mPicker;
    TWeakObjectPtr<AActor> mPickedActor;
};
