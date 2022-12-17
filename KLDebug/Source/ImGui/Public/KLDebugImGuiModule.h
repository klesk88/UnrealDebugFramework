#pragma once

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FKLDebugImGuiFeatureManager;

// main editor module
class KLDEBUGIMGUI_API FKLDebugImGuiModule final : public IModuleInterface
{
public:
    // IModuleInterface
    void StartupModule() final;
    void ShutdownModule() final;
    //

    UE_NODISCARD static FKLDebugImGuiModule& Get();
};
