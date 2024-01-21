#pragma once

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class KLDEBUGUSERTHIRDPARTYIMGUINODEEDITOR_API FKLDebugUserThirdPartyImGuiNodeEditorModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    //
};
