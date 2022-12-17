#pragma once

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FKLDebugWindowManager;

// main editor module
class KLDEBUGRUNTIME_API FKLDebugRuntimeModule final : public IModuleInterface
{
public:
    // IModuleInterface
    void StartupModule() final;
    void ShutdownModule() final;
    //

    UE_NODISCARD static FKLDebugRuntimeModule& Get();
};
