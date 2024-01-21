#pragma once

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class KLDEBUGUSERTHIRDPARTYIMGUI_API FKLDebugUserThirdPartyImGuiModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
    //
};
