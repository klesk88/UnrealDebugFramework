#include "Window/KLDebugImGuiWindow.h"

//ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"

void FKLDebugImGuiWindow::Update()
{
    if (!ImGui::BeginTabBar("Selection", ImGuiTabBarFlags_::ImGuiTabBarFlags_None))
    {
        return;
    }

    if (ImGui::BeginTabItem("Engine"))
    {
        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
}
