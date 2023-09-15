#include "Feature/Interface/Private/KLDebugImGuiFeatureInterface_Subsystem.h"

#include "Helpers/KLDebugImGuiHelpers.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

void IKLDebugImGuiFeatureInterface_Subsystem::DrawImGui(const FKLDebugImGuiFeatureInterfaceImGuiContext_Subsystem& _Context)
{
    auto Callback = [this, &_Context]()->void {
        DrawImGuiChild(_Context);
    };

    KL::Debug::ImGuiHelpers::CreateWindowCallCbk(TCHAR_TO_ANSI(*GetWindowName()),
        _Context.GetIsWindowOpen(),
        Callback);
}
