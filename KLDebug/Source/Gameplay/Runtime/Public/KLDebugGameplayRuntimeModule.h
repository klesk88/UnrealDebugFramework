#pragma once

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class KLDEBUGGAMEPLAYRUNTIME_API FKLDebugGameplayRuntimeModule final : public IModuleInterface
{
public:
    // IModuleInterface
    void StartupModule() final;
    void ShutdownModule() final;
    //

    UE_NODISCARD static FKLDebugGameplayRuntimeModule& GetMutable();
    UE_NODISCARD static const FKLDebugGameplayRuntimeModule& Get();
};

inline const FKLDebugGameplayRuntimeModule& FKLDebugGameplayRuntimeModule::Get()
{
    return GetMutable();
}

