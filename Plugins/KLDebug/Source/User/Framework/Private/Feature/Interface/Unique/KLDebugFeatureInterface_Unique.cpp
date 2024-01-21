// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Unique/KLDebugFeatureInterface_Unique.h"

#include "Feature/Interface/Unique/Input/KLDebugFeatureImGuiInput_Unique.h"
#include "ThirdParty/ImGui/Helpers/KLDebugImGuiHelpers.h"

// modules
#include "User/ThirdParty/ImGui/Public/Library/imgui.h"

void IKLDebugFeatureInterface_Unique::DrawImGui(FKLDebugFeatureImGuiInput_Unique& _Input)
{
    auto Callback = [this, &_Input]() -> void {
        DrawImGuiChild(_Input);
    };

    KL::Debug::ImGuiHelpers::CreateWindowCallCbk(TCHAR_TO_ANSI(*GetWindowName()),
        _Input.GetIsWindowOpen(),
        Callback);
}
