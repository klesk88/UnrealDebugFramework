#include "Input/KLDebugEditorInputManager.h"

// KLDebugGameplay module
#include "Gameplay/Public/Input/KLDebugGameplayInputHelpers.h"
// KLDebugUtils module
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Editor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputEditorSubsystem.h"
#include "GameFramework/InputSettings.h"

// #include "Commands/KLDebugEditorCommands.h"
//  KLImGui module
//  #include "ImGui/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
//  #include "Framework/Commands/UIAction.h"
//  #include "Framework/Commands/UICommandList.h"
//  #include "Kismet2/DebuggerCommands.h"

void FKLDebugEditorInputManager::Init()
{
    // FKLDebugEditorInputManager::Register();

    // CommandList = FPlayWorldCommands::GlobalPlayWorldActions;
    // CommandList->MapAction(FKLDebugEditorInputManager::Get().GetEnableImGuiCmd(),
    //                        FExecuteAction::CreateRaw(this, &FKLDebugEditorInputManager::OnEnableImGui));

    const UClass* InputClassToSpawn = UEnhancedInputComponent::StaticClass();
    if (UInputSettings::GetDefaultInputComponentClass() && UInputSettings::GetDefaultInputComponentClass()->IsChildOf(UEnhancedInputComponent::StaticClass()))
    {
        InputClassToSpawn = UInputSettings::GetDefaultInputComponentClass();
    }

    mEditorInputComponent = NewObject<UInputComponent>(GetTransientPackage(), InputClassToSpawn, TEXT("KLDebugGameplayEditor"), RF_Transient);
    if (mEditorInputComponent)
    {
        mEditorInputComponent->AddToRoot();
    }

    RegisterInputComponent();
}

void FKLDebugEditorInputManager::Shutdown()
{
    // FKLDebugGameplayEditorCommands::Unregister();

    UnregisterInputComponent();

    if (mEditorInputComponent)
    {
        mEditorInputComponent->RemoveFromRoot();
    }

    mEditorInputComponent = nullptr;
}

void FKLDebugEditorInputManager::RegisterInputComponent() const
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

void FKLDebugEditorInputManager::UnregisterInputComponent() const
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

UEnhancedInputEditorSubsystem* FKLDebugEditorInputManager::GetInputEditorSubsystem() const
{
    if (GEditor)
    {
        return GEditor->GetEditorSubsystem<UEnhancedInputEditorSubsystem>();
    }

    return nullptr;
}

// void FKLDebugEditorInputManager::OnEnableImGui()
//{
//     UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
//     if (!ImGuiEngineSubsystem)
//     {
//         return;
//     }
//
//     ImGuiEngineSubsystem->ToogleImGuiSystemState();
// }