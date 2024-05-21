// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/ThirdParty/ImGui/Public/Library/imgui.h"

// engine
#include "Containers/StringConv.h"
#include "Containers/UnrealString.h"
#include "HAL/Platform.h"
#include "Math/Color.h"

class UWorld;

namespace KL::Debug::ImGuiHelpers
{
    KLDEBUGUSERFRAMEWORK_API void HelpMarker(const char* _Description);
    [[nodiscard]] KLDEBUGUSERFRAMEWORK_API ImVec4 ConvertUnrealColorToImGui(const FColor& _Color);
    [[nodiscard]] KLDEBUGUSERFRAMEWORK_API ImVec4 ConvertUnrealColorToImGui(const FLinearColor& _Color);
    KLDEBUGUSERFRAMEWORK_API void DrawColoredText(const FColor _Color, const FString& _Text);
    KLDEBUGUSERFRAMEWORK_API void DrawConditionalColoredText(const FString& _Text, const bool _ConditionResult);

    template <typename CallbackType>
    void CreateWindowCallCbk(const ANSICHAR* _WindowName, bool& _IsOpen, const CallbackType& _Callback)
    {
        const ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysHorizontalScrollbar;
        if (!::ImGui::Begin(_WindowName, &_IsOpen, WindowFlags))
        {
            ::ImGui::End();
            return;
        }

        _Callback();
        ::ImGui::End();
    }

}    // namespace KL::Debug::ImGuiHelpers