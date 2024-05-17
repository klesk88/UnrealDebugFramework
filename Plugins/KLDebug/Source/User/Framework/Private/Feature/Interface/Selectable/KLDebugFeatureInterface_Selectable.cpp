// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Selectable/KLDebugFeatureInterface_Selectable.h"

#include "Feature/Interface/Selectable/Input/KLDebugFeatureImGuiInput_Selectable.h"
#include "ThirdParty/ImGui/Helpers/KLDebugImGuiHelpers.h"

// modules
#include "User/ThirdParty/ImGui/Public/Library/imgui.h"

void IKLDebugFeatureInterface_Selectable::DrawImGui(FKLDebugFeatureImGuiInput_Selectable& _Input)
{
    auto Callback = [this, &_Input]() -> void {
        DrawImGuiChild(_Input);
    };

    KL::Debug::ImGuiHelpers::CreateWindowCallCbk(TCHAR_TO_ANSI(*GetWindowName()),
        _Input.GetIsWindowOpen(),
        Callback);
}
