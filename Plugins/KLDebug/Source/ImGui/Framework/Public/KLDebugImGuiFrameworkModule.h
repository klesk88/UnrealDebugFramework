// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FString;

// main editor module
class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFrameworkModule final : public IModuleInterface
{
public:
    // IModuleInterface
    void StartupModule() final;
    void ShutdownModule() final;
    //

    [[nodiscard]] static FKLDebugImGuiFrameworkModule& Get();

    [[nodiscard]] const FString& GetKLDebugEngineFlag() const;
};
