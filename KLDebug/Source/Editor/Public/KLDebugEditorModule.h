#pragma once

#include "Input/KLDebugEditorInputManager.h"
#include "Picker/KLDebugEditorPicker.h"
#include "Windows/KLDebugEditorMainWindow.h"

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

// main editor module
class KLDEBUGEDITOR_API KLDebugEditorModule final : public IModuleInterface
{
public:
    // IModuleInterface
    void StartupModule() final;
    void ShutdownModule() final;
    //

    UE_NODISCARD static KLDebugEditorModule& Get();

private:
    FKLDebugEditorInputManager mInputManager;
    FKLDebugEditorMainWindow   mMainImGuiWindow;
    FKLDebugEditorPicker       mEditorPicker;
};
