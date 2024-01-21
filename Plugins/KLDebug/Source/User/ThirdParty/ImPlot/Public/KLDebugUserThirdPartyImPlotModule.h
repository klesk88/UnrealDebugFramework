#pragma once

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class KLDEBUGUSERTHIRDPARTYIMPLOT_API FKLDebugUserThirdPartyImPlotModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    //
};
