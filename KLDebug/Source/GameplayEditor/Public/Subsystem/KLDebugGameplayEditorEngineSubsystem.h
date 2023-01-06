#pragma once

// engine
#include "Components/InputComponent.h"
#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "UObject/ObjectPtr.h"

#include "KLDebugGameplayEditorEngineSubsystem.generated.h"

class UEnhancedInputEditorSubsystem;

UCLASS(Transient)
class KLDEBUGGAMEPLAYEDITOR_API UKLDebugGameplayEditorEngineSubsystem final : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;

private:
    void RegisterInputComponent() const;
    void UnregisterInputComponent() const;

    UE_NODISCARD UEnhancedInputEditorSubsystem* GetInputEditorSubsystem() const;

private:
    UPROPERTY(Transient)
    TObjectPtr<UInputComponent> mEditorInputComponent;
};
