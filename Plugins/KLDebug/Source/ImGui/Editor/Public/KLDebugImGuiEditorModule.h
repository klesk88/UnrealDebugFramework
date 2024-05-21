// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Extender/KLDebugEditorMenuExtender.h"
#include "Picker/KLDebugEditorPicker.h"

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

    [[nodiscard]] static FKLDebugImGuiEditorModule& Get();

private:
    FKLDebugEditorPicker mEditorPicker;
    FKLDebugEditorMenuExtender mMenuExtender;
};
