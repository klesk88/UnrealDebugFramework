#include "Helpers/KLDebugImGuiHelpers.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

namespace KL::Debug::ImGuiHelpers
{
    void HelpMarker(const char* _Description)
    {
        // Helper to display a little (?) mark which shows a tooltip when hovered.
        // In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)

        // ImGui::TextDisabled("(?)");
        // if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
        //{
        //     ImGui::BeginTooltip();
        //     ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        //     ImGui::TextUnformatted(_Description);
        //     ImGui::PopTextWrapPos();
        //     ImGui::EndTooltip();
        // }
    }
}  // namespace KL::Debug::ImGuiHelpers