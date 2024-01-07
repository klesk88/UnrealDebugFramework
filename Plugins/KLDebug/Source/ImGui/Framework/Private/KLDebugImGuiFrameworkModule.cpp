// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "KLDebugImGuiFrameworkModule.h"

// engine
#include "Containers/UnrealString.h"
#include "Debug/DebugDrawService.h"
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLDebugImGuiFramework"

namespace KL::Debug::ImGui::Module
{
    static const FName Module(TEXT("KLDebugImGuiFramework"));
    static const FString CustomEngineFlag(TEXT("KLDebugFrameworkFlag"));
}    // namespace  KL::Debug::ImGui::Module

////////////////////////////////////////////////////////////////////////////

void FKLDebugImGuiFrameworkModule::StartupModule()
{
    FEngineShowFlags::RegisterCustomShowFlag(*KL::Debug::ImGui::Module::CustomEngineFlag, false, EShowFlagGroup::SFG_Transient, LOCTEXT("KLDebugFrameworkFlag", "KLDebugFrameworkFlag"));
}

void FKLDebugImGuiFrameworkModule::ShutdownModule()
{
}

FKLDebugImGuiFrameworkModule& FKLDebugImGuiFrameworkModule::Get()
{
    return FModuleManager::GetModuleChecked<FKLDebugImGuiFrameworkModule>(KL::Debug::ImGui::Module::Module);
}

const FString& FKLDebugImGuiFrameworkModule::GetKLDebugEngineFlag() const
{
    return KL::Debug::ImGui::Module::CustomEngineFlag;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugImGuiFrameworkModule, KLDebugImGuiFramework)