#include "KLDebugImGuiModule.h"

// engine
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLDebugImGui"

namespace KL::Debug::ImGui::Module
{
    static const FName Module(TEXT("KLDebugImGui"));
}  // namespace  KL::Debug::ImGui::Module

////////////////////////////////////////////////////////////////////////////

void FKLDebugImGuiModule::StartupModule()
{
}

void FKLDebugImGuiModule::ShutdownModule()
{
}

FKLDebugImGuiModule& FKLDebugImGuiModule::Get()
{
    return FModuleManager::GetModuleChecked<FKLDebugImGuiModule>(KL::Debug::ImGui::Module::Module);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugImGuiModule, KLDebugImGui)