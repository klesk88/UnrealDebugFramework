#include "Input/KLDebugGameplayInputHelpers.h"

//KLDebugImGui module
#include "ImGui/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"

// engine
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "GenericPlatform/GenericApplication.h"
#include "InputCoreTypes.h"

namespace KL::Debug::Gameplay::Input::Helpers
{
    //////////////////////////////////////////////////////////////////////////////////////
    /// private section
    void BindMainInputsToEnhancedConmponent(UEnhancedInputComponent& _OutEnhancedComponent)
    {
    }

    //////////////////////////////////////////////////////////////////////////////////////
    /// public section

    void BindMainInputsToComponent(UInputComponent& _OutInputComponent)
    {
        if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(&_OutInputComponent))
        {
            BindMainInputsToEnhancedConmponent(*EnhancedInput);
            return;
        }

        UKLDebugImGuiEngineSubsystem* EngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
        if (!EngineSubsystem)
        {
            return;
        }

        _OutInputComponent.BindKey(FInputChord(EKeys::D, EModifierKey::Alt),
                                   EInputEvent::IE_Pressed,
                                   EngineSubsystem,
                                   &UKLDebugImGuiEngineSubsystem::ToogleImGuiSystemState);
    }
}