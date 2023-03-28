#pragma once

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/StringConv.h"
#include "Containers/UnrealString.h"
#include "HAL/Platform.h"
#include "Math/Color.h"

namespace KL::Debug::ImGuiHelpers
{
    KLDEBUGIMGUI_API void                HelpMarker(const char* _Description);
    KLDEBUGIMGUI_API UE_NODISCARD ImVec4 ConvertUnrealColorToImGui(const FColor& _Color);
}  // namespace KL::Debug::ImGuiHelpers