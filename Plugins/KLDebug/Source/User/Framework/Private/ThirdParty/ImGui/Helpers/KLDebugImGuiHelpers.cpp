// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ThirdParty/ImGui/Helpers/KLDebugImGuiHelpers.h"

// modules
#include "User/ThirdParty/ImGui/Public/Library/imgui.h"

namespace KL::Debug::ImGuiHelpers
{
    void HelpMarker(const char* _Description)
    {
        // Helper to display a little (?) mark which shows a tooltip when hovered.
        // In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)

        ::ImGui::TextDisabled("(?)");
        if (::ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
        {
            ::ImGui::BeginTooltip();
            ::ImGui::PushTextWrapPos(::ImGui::GetFontSize() * 35.0f);
            ::ImGui::TextUnformatted(_Description);
            ::ImGui::PopTextWrapPos();
            ::ImGui::EndTooltip();
        }
    }

    ImVec4 ConvertUnrealColorToImGui(const FColor& _Color)
    {
        static constexpr float InvValue = 1.f / 255.f;
        return ImVec4(_Color.R * InvValue, _Color.G * InvValue, _Color.B * InvValue, _Color.A * InvValue);
    }

    ImVec4 ConvertUnrealColorToImGui(const FLinearColor& _Color)
    {
        static constexpr float InvValue = 1.f / 255.f;
        return ImVec4(_Color.R * InvValue, _Color.G * InvValue, _Color.B * InvValue, _Color.A * InvValue);
    }

    void DrawColoredText(const FColor _Color, const FString& _Text)
    {
        ::ImGui::TextColored(ConvertUnrealColorToImGui(_Color), "%s", TCHAR_TO_ANSI(*_Text));
    }

    void DrawConditionalColoredText(const FString& _Text, const bool _ConditionResult)
    {
        const FColor Color = _ConditionResult ? FColor::Green : FColor::Red;

        ::ImGui::Text("%s: ", TCHAR_TO_ANSI(*_Text));
        ::ImGui::SameLine();
        ::ImGui::TextColored(ConvertUnrealColorToImGui(Color), "%s", _ConditionResult ? "TRUE" : "FALSE");
    }

}    // namespace KL::Debug::ImGuiHelpers