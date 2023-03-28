#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"

#include "Helpers/KLDebugImGuiHelpers.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

void IKLDebugImGuiFeatureInterface_SelectableObject::DrawImGui(const UWorld& _World, UObject& _Object)
{
    const ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings;
    if (!ImGui::Begin(TCHAR_TO_ANSI(*GetWindowName()), nullptr, WindowFlags))
    {
        return;
    }

    DrawImGuiChild(_World, _Object);
    ImGui::End();
}
