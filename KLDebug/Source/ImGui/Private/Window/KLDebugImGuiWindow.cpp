#include "Window/KLDebugImGuiWindow.h"

#include "Window/KLDebugImGuiWindowDelegates.h"

//ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"
//https://github.com/ocornut/imgui/issues/3518
#include "ImGuiThirdParty/Public/Library/imgui_internal.h"

//engine
#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void FKLDebugImGuiWindow::Update(const UWorld& _World)
{
    DrawImGuiTopBar(_World);
    DrawImGuiBar();
    DrawImGuiBottomBar(_World);
}

void FKLDebugImGuiWindow::DrawImGuiTopBar(const UWorld& _World) const
{
    if (!ImGui::BeginMainMenuBar())
    {
        return;
    }

    if (ImGui::BeginMenu("Test"))
    {
        ImGui::EndMenu();
    }

    KL::Debug::ImGui::MainWindow::Delegate::OnDrawTopBarDelegate.Broadcast(_World);

    ImGui::EndMainMenuBar();
}

void FKLDebugImGuiWindow::DrawImGuiBottomBar(const UWorld& _World) const
{
    if (!mDrawBottomBar)
    {
        return;
    }

    ImGuiViewportP* Viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
    const ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    const float Height = ImGui::GetFrameHeight();

    //FIXME: Need to fix when we resize the editor viewport after pressing f8. In that case it seems to don't
    //scale for some reason
    if (!ImGui::BeginViewportSideBar("##SecondaryMenuBar", Viewport, ImGuiDir_Down, Height, WindowFlags))
    {
        return;
    }

    if (!ImGui::BeginMenuBar())
    {
        ImGui::End();  
        return;
    }

    ImGui::Text("Frame: [%llu]", UKismetSystemLibrary::GetFrameCount());
    ImGui::SameLine();
    ImGui::Text(" Time: [%.3f]", _World.GetTimeSeconds());

    const APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(&_World, 0);
    if (CameraManager)
    {
        ImGui::SameLine();
        ImGui::Text(" Player: [%ls]", *CameraManager->GetCameraLocation().ToString());
    }

    ImGui::SameLine();
    ImGui::Text(" FPS: [%.3f]", _World.GetDeltaSeconds() != 0.f ? 1.f / _World.GetDeltaSeconds() : 0.f);

    KL::Debug::ImGui::MainWindow::Delegate::OnDrawBottomBarDelegate.Broadcast(_World);

    ImGui::EndMenuBar();
    ImGui::End();
}

void FKLDebugImGuiWindow::DrawImGuiBar() const
{
    if (!ImGui::BeginTabBar("Selection", ImGuiTabBarFlags_::ImGuiTabBarFlags_None))
    {
        return;
    }

    DrawImGuiEngine();

    ImGui::EndTabBar();
}

void FKLDebugImGuiWindow::DrawImGuiEngine() const
{
    if (ImGui::BeginTabItem("Engine"))
    {
        ImGui::EndTabItem();
    }
}
