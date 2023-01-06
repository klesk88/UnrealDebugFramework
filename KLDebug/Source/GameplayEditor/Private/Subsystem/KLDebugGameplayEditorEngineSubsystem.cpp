#include "Subsystem/KLDebugGameplayEditorEngineSubsystem.h"

// KLDebugGameplay module
#include "Gameplay/Public/Input/KLDebugGameplayInputHelpers.h"
// KLDebugUtils module
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Editor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputEditorSubsystem.h"
#include "GameFramework/InputSettings.h"

void UKLDebugGameplayEditorEngineSubsystem::Initialize(FSubsystemCollectionBase& _Collection)
{
    const UClass* InputClassToSpawn = UEnhancedInputComponent::StaticClass();
    if (UInputSettings::GetDefaultInputComponentClass() && UInputSettings::GetDefaultInputComponentClass()->IsChildOf(UEnhancedInputComponent::StaticClass()))
    {
        InputClassToSpawn = UInputSettings::GetDefaultInputComponentClass();
    }

    mEditorInputComponent = NewObject<UInputComponent>(this, InputClassToSpawn, TEXT("KLDebugGameplayEditor"), RF_Transient);

    RegisterInputComponent();
}

void UKLDebugGameplayEditorEngineSubsystem::Deinitialize()
{
    UnregisterInputComponent();

    mEditorInputComponent = nullptr;
}

void UKLDebugGameplayEditorEngineSubsystem::RegisterInputComponent() const
{
    ensureMsgf(mEditorInputComponent, TEXT("this must still be valid at this point"));

    if (UEnhancedInputEditorSubsystem* EditorInputSubsystem = GetInputEditorSubsystem())
    {
        EditorInputSubsystem->PushInputComponent(mEditorInputComponent.Get());
        EditorInputSubsystem->StartConsumingInput();

        UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(mEditorInputComponent.Get());
        KL::Debug::Gameplay::Input::Helpers::BindMainInputsToComponent(*EditorInputSubsystem, *EnhancedInputComponent);
    }
    else
    {
        UE_LOG(LogKL_Debug, Error, TEXT("No UEnhancedInputEditorSubsystem found"));
    }
}

void UKLDebugGameplayEditorEngineSubsystem::UnregisterInputComponent() const
{
    ensureMsgf(mEditorInputComponent, TEXT("this must still be valid at this point"));

    // Removes the component editor if possible
    //
    // Note: Despite the functions being called "Pop" it's actually just removing our specific input component from
    // the stack rather than blindly popping the top component
    if (UEnhancedInputEditorSubsystem* EditorInputSubsystem = GetInputEditorSubsystem())
    {
        EditorInputSubsystem->PopInputComponent(mEditorInputComponent.Get());
    }
}

UEnhancedInputEditorSubsystem* UKLDebugGameplayEditorEngineSubsystem::GetInputEditorSubsystem() const
{
    if (GEditor)
    {
        return GEditor->GetEditorSubsystem<UEnhancedInputEditorSubsystem>();
    }

    return nullptr;
}
