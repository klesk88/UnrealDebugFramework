#include "NetImGuiThirdPartyModule.h"
#include "Commands/ImUnrealCommand.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

#define LOCTEXT_NAMESPACE "NetImGuiThirdPartyModule"

namespace KL::NetImGui::Module
{
    static const FName Module(TEXT("NetImGuiThirdParty"));

#if IMGUI_UNREAL_COMMAND_ENABLED
    static ImUnrealCommand::CommandContext* mUnrealCommandContext = nullptr;
#endif
}  // namespace  KL::UnrealImGui::Module

///////////////////////////////////////////////////////////////////////

void FNetImGuiThirdPartyModule::StartupModule()
{
#if IMGUI_UNREAL_COMMAND_ENABLED
    KL::NetImGui::Module::mUnrealCommandContext = ImUnrealCommand::Create();  // Create a new Imgui Command Window
#endif
}

void FNetImGuiThirdPartyModule::ShutdownModule()
{
#if IMGUI_UNREAL_COMMAND_ENABLED
    ImUnrealCommand::Destroy(KL::NetImGui::Module::mUnrealCommandContext);
#endif
}

const FNetImGuiThirdPartyModule& FNetImGuiThirdPartyModule::Get()
{
    return FModuleManager::GetModuleChecked<FNetImGuiThirdPartyModule>(KL::NetImGui::Module::Module);
}

void FNetImGuiThirdPartyModule::DrawCommands() const
{
    //----------------------------------------------------------------------------
    // Display a 'Unreal Console Command' menu entry in MainMenu bar, and the
    // 'Unreal Console command' window itself when requested
    //----------------------------------------------------------------------------
#if IMGUI_UNREAL_COMMAND_ENABLED
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("NetImgui"))
        {
            ImGui::MenuItem("Unreal-Commands", nullptr, &ImUnrealCommand::IsVisible(KL::NetImGui::Module::mUnrealCommandContext));
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Always try displaying the 'Unreal Command Imgui' Window (handle Window visibility internally)
    ImUnrealCommand::Show(KL::NetImGui::Module::mUnrealCommandContext);
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNetImGuiThirdPartyModule, NetImGuiThirdParty)
