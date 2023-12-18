// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"

#include "Helpers/KLDebugImGuiHelpers.h"

// modules
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

void IKLDebugImGuiFeatureInterface_Unique::DrawImGui(const FKLDebugImGuiFeatureImGuiInput_Unique& _Input)
{
    auto Callback = [this, &_Input]() -> void {
        DrawImGuiChild(_Input);
    };

    KL::Debug::ImGuiHelpers::CreateWindowCallCbk(TCHAR_TO_ANSI(*GetWindowName()),
        _Input.GetIsWindowOpen(),
        Callback);
}
