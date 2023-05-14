#pragma once

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/StringConv.h"
#include "Containers/UnrealString.h"
#include "HAL/Platform.h"
#include "Math/Color.h"

class UWorld;

namespace KL::Debug::ImGuiHelpers
{
    KLDEBUGIMGUI_API void                HelpMarker(const char* _Description);
    KLDEBUGIMGUI_API UE_NODISCARD ImVec4 ConvertUnrealColorToImGui(const FColor& _Color);

    template<typename CallbackType>
    void CreateWindowCallCbk(const ANSICHAR* _WindowName, bool& _IsOpen, const CallbackType& _Callback)
    {
        const ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings;
        if (!::ImGui::Begin(_WindowName, &_IsOpen, WindowFlags))
        {
            return;
        }

        _Callback();
        ::ImGui::End();
    }

}  // namespace KL::Debug::ImGuiHelpers