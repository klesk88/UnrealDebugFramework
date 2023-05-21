#pragma once

//engine
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IMPLOTTHIRDPARTY_API FImPlotThirdPartyModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    //
};
