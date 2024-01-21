#include "NetImGuiThirdPartyModule.h"

// modules
#include "User/ThirdParty/ImGui/Public/Library/imgui.h"

#define LOCTEXT_NAMESPACE "NetImGuiThirdPartyModule"

namespace KL::NetImGui::Module
{
    static const FName Module(TEXT("NetImGuiThirdParty"));
}  // namespace  KL::UnrealImGui::Module

///////////////////////////////////////////////////////////////////////

void FNetImGuiThirdPartyModule::StartupModule()
{
}

void FNetImGuiThirdPartyModule::ShutdownModule()
{
}

const FNetImGuiThirdPartyModule& FNetImGuiThirdPartyModule::Get()
{
    return FModuleManager::GetModuleChecked<FNetImGuiThirdPartyModule>(KL::NetImGui::Module::Module);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNetImGuiThirdPartyModule, NetImGuiThirdParty)
