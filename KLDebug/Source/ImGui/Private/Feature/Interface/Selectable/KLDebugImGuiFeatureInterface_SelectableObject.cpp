#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"

#include "Helpers/KLDebugImGuiHelpers.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

void IKLDebugImGuiFeatureInterface_SelectableObject::DrawImGui(const FKLDebugImGuiFeatureInterfaceImGuiContext_Selectable& _Context)
{
    auto Callback = [this, &_Context]()->void {
        DrawImGuiChild(_Context);
    };

    KL::Debug::ImGuiHelpers::CreateWindowCallCbk(TCHAR_TO_ANSI(*GetWindowName()),
        _Context.GetIsWindowOpen(),
        Callback);
}
