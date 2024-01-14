// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

// modules
#include "User/Framework/Public/Input/Delegates/KLDebugUserInputDelegates.h"

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class KLDEBUGEXAMPLES_API FKLDebugExamplesModule final : public IModuleInterface
{
public:
    // IModuleInterface
    void StartupModule() final;
    void ShutdownModule() final;
    //

private:
    EKLDebugInputReply OnPauseDelegate(const KL::Debug::User::Gameplay::Input::FOnInputTriggerData& _Data);
};