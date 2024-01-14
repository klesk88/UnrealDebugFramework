// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Mode/KLDebugExampleMode.h"

// modules
#include "ImGui/User/Public/Helpers/KLDebugImGuiHelpers.h"
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"
#include "User/Framework/Public/Mode/KLDebugModeInterfaceAllInput.h"

KL_DEBUG_CREATE_MODE(FKLDebugExampleMode)

const FString& FKLDebugExampleMode::GetFriendlyName() const
{
    static FString Name(TEXT("ExampleMode"));
    return Name;
}

void FKLDebugExampleMode::DrawImGui(const FKLDebugModeDrawImGuiInput& _Input)
{
    ImGuiViewport* Viewport = ImGui::GetMainViewport();
    ImDrawList* DrawList = ImGui::GetForegroundDrawList();
    DrawList->AddCircle(Viewport->GetCenter(), 10.f, ImColor(1.0f, 0.0f, 0.0f), 10, 2.f);

    ImGui::SetCursorPosY(200.f);
    ImGui::Text("example imgui text");
}

void FKLDebugExampleMode::DrawCanvas(FKLDebugModeDrawCanvasInput& _Input) const
{
    _Input.Printf(TEXT("Example Canvas text: {%s}%d"), TEXT("red"), 1);
}
