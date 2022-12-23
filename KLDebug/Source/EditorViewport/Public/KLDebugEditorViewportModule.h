#pragma once

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Templates/SharedPointer.h"

// main editor module
class KLDEBUGEDITORVIEWPORT_API FKLDebugEditorViewportModule final : public IModuleInterface
{
public:
    // IModuleInterface
    void StartupModule() final;
    void ShutdownModule() final;
    //

    static UE_NODISCARD FKLDebugEditorViewportModule& Get();

private:
    void ActivateDebugViewport();
    void RegisterMenuExtensions();
    void RegisterLevelEditorExtensions();

    void UnregisterLevelEditorExtensions();
    void UnregisterMenuExtensions();
};
