// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "KLUnrealImGuiModule.h"

// modules
#include "ThirdParty/UnrealImGui/Public/ImGuiContextManager.h"
#include "ThirdParty/UnrealImGui/Public/ImGuiModuleManager.h"
#include "ThirdParty/UnrealImGui/Public/ThirdPartyBuildNetImgui.h"
#include "User/ThirdParty/ImGui/Public/Library/imgui.h"
#include "User/ThirdParty/ImPlot/Public/Library/implot.h"
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLUnrealImGui"

namespace KL::UnrealImGui::Module
{
    static const FName Module(TEXT("KLUnrealImGui"));
}    // namespace  KL::UnrealImGui::Module

////////////////////////////////////////////////////////////////////////////

void FKLUnrealImGuiModule::StartupModule()
{
    FUnrealImGuiModule::Init();

    FImGuiModuleManager& ModuleManager = GetImguiModuleManager();
    FImGuiContextManager& ContextManager = ModuleManager.GetContextManager();

    mOnContextProxyCreated = ContextManager.OnContextProxyCreated.AddRaw(this, &FKLUnrealImGuiModule::OnContextProxyCreated);
    mOnNetImGuiContextCreated = ThirdParty::UnrealImGui::NetImGui::OnNetImGuiCreatedContext.AddRaw(this, &FKLUnrealImGuiModule::OnNetImGuiContextCreated);

    mImGuiFontManager.Init(ContextManager);
    ContextManager.Init();
}

void FKLUnrealImGuiModule::ShutdownModule()
{
    FImGuiModuleManager& ModuleManager = GetImguiModuleManager();
    FImGuiContextManager& ContextManager = ModuleManager.GetContextManager();

    if (mOnContextProxyCreated.IsValid())
    {
        ContextManager.OnContextProxyCreated.Remove(mOnContextProxyCreated);
        mOnContextProxyCreated.Reset();
    }

    if (mOnNetImGuiContextCreated.IsValid())
    {
        ThirdParty::UnrealImGui::NetImGui::OnNetImGuiCreatedContext.Remove(mOnNetImGuiContextCreated);
        mOnNetImGuiContextCreated.Reset();
    }

    mImGuiFontManager.Shutdown(ContextManager);

    FUnrealImGuiModule::Shutdown();
}

FKLUnrealImGuiModule& FKLUnrealImGuiModule::Get()
{
    return FModuleManager::GetModuleChecked<FKLUnrealImGuiModule>(KL::UnrealImGui::Module::Module);
}

FKLUnrealImGuiModule* FKLUnrealImGuiModule::TryGet()
{
    return FModuleManager::GetModulePtr<FKLUnrealImGuiModule>(KL::UnrealImGui::Module::Module);
}

void FKLUnrealImGuiModule::EnableImGuiSystem()
{
    FImGuiModuleManager& ModuleManager = GetImguiModuleManager();
    ModuleManager.Init();

    mOnEnableStateChangeDelegate.Broadcast(true);
}

void FKLUnrealImGuiModule::DisableImGuiSystem()
{
    FImGuiModuleManager& ModuleManager = GetImguiModuleManager();
    ModuleManager.Shutdown();

    mOnEnableStateChangeDelegate.Broadcast(false);
}

void FKLUnrealImGuiModule::OnContextProxyCreated(int32 _Index, FImGuiContextProxy& _Proxy) const
{
    OnContextCreated();
}

void FKLUnrealImGuiModule::OnNetImGuiContextCreated() const
{
    OnContextCreated();
}

void FKLUnrealImGuiModule::OnContextCreated() const
{
    if (!ImGui::GetCurrentContext())
    {
        UE_LOG(LogKL_Debug, Error, TEXT("ImGui context is not ready to be initialized"));
        return;
    }

    ImGuiIO& IO = ImGui::GetIO();
    IO.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;

    mImGuiFontManager.SetDefaultFont(KL::UnrealImGui::Fonts::eFont::kCousineFixed16);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLUnrealImGuiModule, KLUnrealImGui)