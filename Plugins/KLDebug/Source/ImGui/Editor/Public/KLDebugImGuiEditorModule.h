// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Extender/KLDebugEditorMenuExtender.h"
#include "Picker/KLDebugEditorPicker.h"
#include "Windows/KLDebugEditorMainWindow.h"

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
    FKLDebugEditorMainWindow mMainImGuiWindow;
    FKLDebugEditorPicker mEditorPicker;
    FKLDebugEditorMenuExtender mMenuExtender;
};
