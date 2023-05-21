#pragma once

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IMGUITHIRDPARTY_API FImGuiThirdPartyModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
    //
};
