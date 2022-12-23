#pragma once

// engine
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class NETIMGUITHIRDPARTY_API FNetImGuiThirdPartyModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
    //

    static const FNetImGuiThirdPartyModule& Get();

    void DrawCommands() const;
};
