#pragma once

#include "Font/KLUnrealImGuiFontManager.h"

//UnrealImGui module
#include "UnrealImGui/Public/UnrealImGuiModule.h"

// engine
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "Modules/ModuleInterface.h"

class FImGuiContextProxy;
class FImGuiModuleManager;

// main editor module
class KLUNREALIMGUI_API FKLUnrealImGuiModule final : public FUnrealImGuiModule
{
public:
    // IModuleInterface
    void StartupModule() final;
    void ShutdownModule() final;
    //

    UE_NODISCARD static FKLUnrealImGuiModule& Get();
    UE_NODISCARD static FKLUnrealImGuiModule* TryGet();

    void EnableImGuiSystem();
    void DisableImGuiSystem();

private:
    void OnContextProxyCreated(int32 _Index, FImGuiContextProxy& _Proxy) const;
    void OnNetImGuiContextCreated() const;
    void OnContextCreated() const;
    
private:
    FKLUnrealImGuiFontManager mImGuiFontManager;
    FDelegateHandle           mOnContextProxyCreated;
    FDelegateHandle           mOnNetImGuiContextCreated;
};
