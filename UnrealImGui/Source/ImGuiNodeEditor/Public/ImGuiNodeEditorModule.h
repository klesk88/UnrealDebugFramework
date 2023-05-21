#pragma once

//engine
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IMGUINODEEDITOR_API FImGuiNodeEditorModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    //
};
