#pragma once

#include "Font/KLUnrealImGuiFontManager.h"

//UnrealImGui module
#include "ThirdParty/UnrealImGui/Public/UnrealImGuiModule.h"

// engine
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "Misc/CoreDelegates.h"
#include "Modules/ModuleInterface.h"

class FImGuiContextProxy;
class FImGuiModuleManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNetImguiEnableStateChange, const bool /*_IsEnable*/);

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
    UE_NODISCARD FDelegateHandle BindToEnableStateChange(const FOnNetImguiEnableStateChange::FDelegate& _Delegate);
    void UnBindFromEnableStateChange(FDelegateHandle& _DelegateHandle);

private:
    void OnContextProxyCreated(int32 _Index, FImGuiContextProxy& _Proxy) const;
    void OnNetImGuiContextCreated() const;
    void OnContextCreated() const;
    
private:
    FKLUnrealImGuiFontManager mImGuiFontManager;
    FDelegateHandle           mOnContextProxyCreated;
    FDelegateHandle           mOnNetImGuiContextCreated;

    FOnNetImguiEnableStateChange mOnEnableStateChangeDelegate;
};

inline FDelegateHandle FKLUnrealImGuiModule::BindToEnableStateChange(const FOnNetImguiEnableStateChange::FDelegate& _Delegate)
{
    return mOnEnableStateChangeDelegate.Add(_Delegate);
}

inline void FKLUnrealImGuiModule::UnBindFromEnableStateChange(FDelegateHandle& _DelegateHandle)
{
    mOnEnableStateChangeDelegate.Remove(_DelegateHandle);
    _DelegateHandle.Reset();
}