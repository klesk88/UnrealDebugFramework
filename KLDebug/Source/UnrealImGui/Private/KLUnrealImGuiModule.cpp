#include "KLUnrealImGuiModule.h"

// UnrealImGui module
#include "UnrealImGui/Public/ImGuiContextManager.h"
#include "UnrealImGui/Public/ImGuiModuleManager.h"

// engine
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLUnrealImGui"

namespace KL::UnrealImGui::Module
{
    static const FName Module(TEXT("KLUnrealImGui"));
}  // namespace  KL::UnrealImGui::Module

////////////////////////////////////////////////////////////////////////////

void FKLUnrealImGuiModule::StartupModule()
{
    FUnrealImGuiModule::Init();

    FImGuiModuleManager& ModuleManager = GetImguiModuleManager();
    FImGuiContextManager& ContextManager = ModuleManager.GetContextManager();
    
    mImGuiFontManager.Init(ContextManager);

    ContextManager.Init();
}

void FKLUnrealImGuiModule::ShutdownModule()
{
    FImGuiModuleManager&  ModuleManager  = GetImguiModuleManager();
    FImGuiContextManager& ContextManager = ModuleManager.GetContextManager();
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
}

void FKLUnrealImGuiModule::DisableImGuiSystem()
{
    FImGuiModuleManager& ModuleManager = GetImguiModuleManager();
    ModuleManager.Shutdown();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLUnrealImGuiModule, KLUnrealImGui)