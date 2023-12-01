// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/StringConv.h"
#include "Containers/UnrealString.h"
#include "HAL/Platform.h"
#include "Math/Color.h"

class UWorld;

namespace KL::Debug::ImGuiHelpers
{
    KLDEBUGIMGUIUSER_API void HelpMarker(const char* _Description);
    KLDEBUGIMGUIUSER_API UE_NODISCARD ImVec4 ConvertUnrealColorToImGui(const FColor& _Color);
    KLDEBUGIMGUIUSER_API UE_NODISCARD ImVec4 ConvertUnrealColorToImGui(const FLinearColor& _Color);
    KLDEBUGIMGUIUSER_API void DrawColoredText(const FColor _Color, const FString& _Text);

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