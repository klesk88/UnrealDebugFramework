#include "Feature/Interface/Private/KLDebugImGuiFeatureInterface_Subsystem.h"

#include "Helpers/KLDebugImGuiHelpers.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

void IKLDebugImGuiFeatureInterface_Subsystem::DrawImGui(const UWorld& _World, bool& _IsWindowOpen)
{
    auto Callback = [this, &_World]()->void {
        DrawImGuiChild(_World);
    };

    KL::Debug::ImGuiHelpers::CreateWindowCallCbk(TCHAR_TO_ANSI(*GetWindowName()),
        _IsWindowOpen,
        Callback);
}
