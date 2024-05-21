// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

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

    [[nodiscard]] static FKLDebugGameplayRuntimeModule& GetMutable();
    [[nodiscard]] static const FKLDebugGameplayRuntimeModule& Get();
};

inline const FKLDebugGameplayRuntimeModule& FKLDebugGameplayRuntimeModule::Get()
{
    return GetMutable();
}
