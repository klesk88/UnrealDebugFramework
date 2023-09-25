#include "Input/KLDebugGameplayInputHelpers.h"

#include "Config/KLDebugGameplayConfig.h"
#include "Input/Config/KLDebugGameplayInputConfig.h"

// KLDebugImGui module
#include "ImGui/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
// KLDebugUtils module
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Components/InputComponent.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GenericPlatform/GenericApplication.h"
#include "InputCoreTypes.h"

namespace KL::Debug::Gameplay::Input::Helpers
{
    //////////////////////////////////////////////////////////////////////////////////////
    /// private section

    IEnhancedInputSubsystemInterface* GetEnhancedInputSubsystemInterface(const ULocalPlayer& _Player)
    {
        return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(&_Player);
    }

    void BindActions(const FKLDebugGameplayInputConfig& _InputConfig, UEnhancedInputComponent& _OutEnhancedComponent, UKLDebugImGuiEngineSubsystem& _ImGuiEngineSusystem)
    {
        if (const UInputAction* InputAction = _InputConfig.TryGetToogleDebug())
        {
            _OutEnhancedComponent.BindAction(InputAction, ETriggerEvent::Triggered, &_ImGuiEngineSusystem, &UKLDebugImGuiEngineSubsystem::ToogleImGuiSystemState);
        }
        else
        {
            UE_LOG(LogKL_Debug, Warning, TEXT("KLDebugGameplay no input action set to enable the debug mode trough player controller"));
            return;
        }
    }

    void BindMappingContext(const UInputMappingContext& _InputMappingContext, const int32 _InputPriority, IEnhancedInputSubsystemInterface& _InputSubsystem)
    {
        if (!_InputSubsystem.HasMappingContext(&_InputMappingContext))
        {
            _InputSubsystem.AddMappingContext(&_InputMappingContext, _InputPriority);
        }
    }

    void BindMainInputsToEnhancedComponent(IEnhancedInputSubsystemInterface& _InputSubsystem, UEnhancedInputComponent& _OutEnhancedComponent, UKLDebugImGuiEngineSubsystem& _ImGuiEngineSusystem)
    {
        const UKLDebugGameplayConfig&      Config      = UKLDebugGameplayConfig::Get();
        const FKLDebugGameplayInputConfig& InputConfig = Config.GetInputConfig();

        const UInputMappingContext* InputMappingContext = InputConfig.TryGetMappingContext();
        if (!InputMappingContext)
        {
            UE_LOG(LogKL_Debug, Warning, TEXT("KLDebugGameplay no mapping contextg set"));
            return;
        }

        BindActions(InputConfig, _OutEnhancedComponent, _ImGuiEngineSusystem);
        BindMappingContext(*InputMappingContext, InputConfig.GetInputContextPriority(), _InputSubsystem);
    }

    void BindMainInputsToNormalComponent(UInputComponent& _OutInputComponent, UKLDebugImGuiEngineSubsystem& _ImGuiEngineSusystem)
    {
        _OutInputComponent.BindKey(FInputChord(EKeys::D, EModifierKey::Alt),
                                   EInputEvent::IE_Pressed,
                                   &_ImGuiEngineSusystem,
                                   &UKLDebugImGuiEngineSubsystem::ToogleImGuiSystemState);
    }

    //////////////////////////////////////////////////////////////////////////////////////
    /// public section

    void BindMainInputsToComponent(const APlayerController& _PlayerController, UInputComponent& _OutInputComponent)
    {
        UKLDebugImGuiEngineSubsystem* KLImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::GetMutable();
        if (!KLImGuiEngineSubsystem)
        {
            ensureMsgf(false, TEXT("we expect to have a valid engine subsystem always"));
            return;
        }

        if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(&_OutInputComponent))
        {
            const ULocalPlayer* Player = _PlayerController.GetLocalPlayer();
            if (!Player)
            {
                UE_LOG(LogKL_Debug, Error, TEXT("KLDebugGameplay no local player found"));
                return;
            }

            IEnhancedInputSubsystemInterface* EnhancedInputSubsystemInterface = GetEnhancedInputSubsystemInterface(*Player);
            if (!EnhancedInputSubsystemInterface)
            {
                UE_LOG(LogKL_Debug, Error, TEXT("KLDebugGameplay no enhanced input subsystem found"));
                return;
            }

            BindMainInputsToEnhancedComponent(*EnhancedInputSubsystemInterface, *EnhancedInput, *KLImGuiEngineSubsystem);
        }
        else
        {
            BindMainInputsToNormalComponent(_OutInputComponent, *KLImGuiEngineSubsystem);
        }
    }

    void BindMainInputsToComponent(IEnhancedInputSubsystemInterface& _InputSubsystem, UEnhancedInputComponent& _InputComponent)
    {
        UKLDebugImGuiEngineSubsystem* KLImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::GetMutable();
        if (!KLImGuiEngineSubsystem)
        {
            ensureMsgf(false, TEXT("we expect to have a valid engine subsystem always"));
            return;
        }

        BindMainInputsToEnhancedComponent(_InputSubsystem, _InputComponent, *KLImGuiEngineSubsystem);
    }

}  // namespace KL::Debug::Gameplay::Input::Helpers