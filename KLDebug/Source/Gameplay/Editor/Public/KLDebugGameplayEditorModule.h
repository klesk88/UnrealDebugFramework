#pragma once

#include "Input/KLDebugEditorInputManager.h"

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

// main editor module
class KLDEBUGGAMEPLAYEDITOR_API FKLDebugGameplayEditorModule final : public IModuleInterface
{
public:
    // IModuleInterface
    void StartupModule() final;
    void ShutdownModule() final;
    //

    UE_NODISCARD static FKLDebugGameplayEditorModule& Get();

private:
    FKLDebugEditorInputManager mInputManager;
};
