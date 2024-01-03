// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "KLDebugExamplesModule.h"

#include "Commands/KLDebugCommand_Pause.h"

// modules
#include "User/Networking/Public/Commands/Helpers/KLDebugUserNetworkingCommandHelpers.h"

// engine
#include "Kismet/KismetSystemLibrary.h"

#define LOCTEXT_NAMESPACE "KLDebugExamplesModule"

void FKLDebugExamplesModule::StartupModule()
{
    KL::Debug::User::Gameplay::Input::BindOnInputPressedPressed(EKLDebugGameplayInputType::TooglePause, FOnKLDebugInputTrigger::CreateRaw(this, &FKLDebugExamplesModule::OnPauseDelegate));
}

void FKLDebugExamplesModule::ShutdownModule()
{
    KL::Debug::User::Gameplay::Input::UnbindOnInputPressedPressed(EKLDebugGameplayInputType::TooglePause);
}

EKLDebugInputReply FKLDebugExamplesModule::OnPauseDelegate(const KL::Debug::User::Gameplay::Input::FOnInputTriggerData& _Data)
{
    if (!UKismetSystemLibrary::IsServer(&_Data.GetOwningWorld()))
    {
        KL::Debug::Networking::Commands::SendCommand<FKLDebugCommand_Pause>(_Data.GetOwningWorld());
        return EKLDebugInputReply::Handled;
    }
    else
    {
        return EKLDebugInputReply::Unhandled;
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugExamplesModule, KLDebugExamples);
