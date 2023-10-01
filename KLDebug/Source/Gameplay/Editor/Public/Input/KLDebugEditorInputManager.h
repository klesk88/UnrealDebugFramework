#pragma once

// engine
#include "Components/InputComponent.h"
#include "CoreMinimal.h"
#include "UObject/ObjectPtr.h"

class UEnhancedInputEditorSubsystem;

class KLDEBUGGAMEPLAYEDITOR_API FKLDebugEditorInputManager
{
public:
    void Init();
    void Shutdown();

private:
    void RegisterInputComponent() const;
    void UnregisterInputComponent() const;

    UE_NODISCARD UEnhancedInputEditorSubsystem* GetInputEditorSubsystem() const;

private:
    TObjectPtr<UInputComponent> mEditorInputComponent;
};
