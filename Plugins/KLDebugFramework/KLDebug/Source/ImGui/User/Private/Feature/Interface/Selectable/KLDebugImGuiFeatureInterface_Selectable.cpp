// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"

#include "Helpers/KLDebugImGuiHelpers.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

void IKLDebugImGuiFeatureInterface_Selectable::DrawImGui(const FKLDebugImGuiFeatureImGuiInput_Selectable& _Input)
{
    auto Callback = [this, &_Input]() -> void {
        DrawImGuiChild(_Input);
    };

    KL::Debug::ImGuiHelpers::CreateWindowCallCbk(TCHAR_TO_ANSI(*GetWindowName()),
        _Input.GetIsWindowOpen(),
        Callback);
}