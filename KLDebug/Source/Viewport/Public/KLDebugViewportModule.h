#pragma once

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Templates/SharedPointer.h"

class SViewport;

// main editor module
class KLDEBUGVIEWPORT_API FKLDebugViewportModule final : public IModuleInterface
{
public:
    // IModuleInterface
    void StartupModule() final;
    void ShutdownModule() final;
    //

    UE_NODISCARD static FKLDebugViewportModule& Get();

private:
    void ActivateDebugViewport();

private:
    TSharedPtr<SViewport> mDebugViewportClient;
};
