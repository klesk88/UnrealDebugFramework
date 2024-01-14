// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Mode/KLDebugExampleMode.h"

// modules
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
    ImGui::Text("ExampleModeImGui");
}

void FKLDebugExampleMode::DrawCanvas(FKLDebugModeDrawCanvasInput& _Input) const
{
    _Input.Printf(TEXT("Example Canvas: {%s}%d"), TEXT("red"), 1);
}
