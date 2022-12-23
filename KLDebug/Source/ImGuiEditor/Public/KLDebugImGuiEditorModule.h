#pragma once

#include "Input/KLDebugImGuiEditorInputManager.h"

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

// main editor module
class KLDEBUGIMGUIEDITOR_API FKLDebugImGuiEditorModule final : public IModuleInterface
{
public:
    // IModuleInterface
    void StartupModule() final;
    void ShutdownModule() final;
    //

    UE_NODISCARD static FKLDebugImGuiEditorModule& Get();

private:
    FKLDebugImGuiEditorInputManager mInputManager;
};
