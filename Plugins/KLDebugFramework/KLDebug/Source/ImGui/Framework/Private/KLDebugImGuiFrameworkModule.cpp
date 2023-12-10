// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "KLDebugImGuiFrameworkModule.h"

// engine
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLDebugImGuiFramework"

namespace KL::Debug::ImGui::Module
{
    static const FName Module(TEXT("KLDebugImGuiFramework"));
}    // namespace  KL::Debug::ImGui::Module

////////////////////////////////////////////////////////////////////////////

void FKLDebugImGuiFrameworkModule::StartupModule()
{
}

void FKLDebugImGuiFrameworkModule::ShutdownModule()
{
}

FKLDebugImGuiFrameworkModule& FKLDebugImGuiFrameworkModule::Get()
{
    return FModuleManager::GetModuleChecked<FKLDebugImGuiFrameworkModule>(KL::Debug::ImGui::Module::Module);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugImGuiFrameworkModule, KLDebugImGuiFramework)