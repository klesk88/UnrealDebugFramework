#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"

#include "Helpers/KLDebugImGuiHelpers.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

void IKLDebugImGuiFeatureInterface_SelectableObject::DrawImGui(const UWorld& _World, bool& _IsWindowOpen, UObject& _Object)
{
    auto Callback = [this, &_World, &_Object]()->void {
        DrawImGuiChild(_World, _Object);
    };

    KL::Debug::ImGuiHelpers::CreateWindowCallCbk(TCHAR_TO_ANSI(*GetWindowName()),
        _IsWindowOpen,
        Callback);
}
