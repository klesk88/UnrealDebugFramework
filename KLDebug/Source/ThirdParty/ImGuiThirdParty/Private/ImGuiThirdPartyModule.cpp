#include "ImGuiThirdPartyModule.h"

#define LOCTEXT_NAMESPACE "ImGuiThirdPartyModule"

void FImGuiThirdPartyModule::StartupModule()
{
}

void FImGuiThirdPartyModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FImGuiThirdPartyModule, ImGuiThirdParty)

