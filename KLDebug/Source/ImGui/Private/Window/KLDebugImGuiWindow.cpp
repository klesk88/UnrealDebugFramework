#include "Window/KLDebugImGuiWindow.h"

#include "Commands/ImUnrealCommand.h"
#include "Window/KLDebugImGuiWindowDelegates.h"

//ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"
//https://github.com/ocornut/imgui/issues/3518
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui_internal.h"

//engine
#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

namespace KL::Debug::ImGuiEditor::MainWindow
{
#if IMGUI_UNREAL_COMMAND_ENABLED
    static ImUnrealCommand::CommandContext* mUnrealCommandContext = nullptr;
#endif
}

void FKLDebugImGuiWindow::Init()
{
#if IMGUI_UNREAL_COMMAND_ENABLED
    KL::Debug::ImGuiEditor::MainWindow::mUnrealCommandContext = ImUnrealCommand::Create();  // Create a new Imgui Command Window

	// Commented code demonstrating how to add/modify Presets
    // Could also modify the list of 'Default Presets' directly (UECommandImgui::sDefaultPresets)
    // ImUnrealcommand::AddPresetFilters(mpImUnrealCommandContext, TEXT("ExamplePreset"), {"ai.Debug", "fx.Dump"});
    // ImUnrealcommand::AddPresetCommands(mpImUnrealCommandContext, TEXT("ExamplePreset"), {"Stat Unit", "Stat Fps"});
#endif
}

void FKLDebugImGuiWindow::Shutdown()
{
#if IMGUI_UNREAL_COMMAND_ENABLED
    ImUnrealCommand::Destroy(KL::Debug::ImGuiEditor::MainWindow::mUnrealCommandContext);
#endif
}

void FKLDebugImGuiWindow::Update(const UWorld& _World)
{
    DrawImGuiTopBar(_World);
    DrawImGuiBottomBar(_World);
    //DrawImGuiBar();

#if IMGUI_UNREAL_COMMAND_ENABLED
    DrawCommands();
#endif
}

void FKLDebugImGuiWindow::DrawImGuiTopBar(const UWorld& _World) const
{
    if (!ImGui::BeginMainMenuBar())
    {
        return;
    }

#if IMGUI_UNREAL_COMMAND_ENABLED
    DrawCommandsMenu();
#endif

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
    ImGui::Text(" FPS: [%.2f]", _World.GetDeltaSeconds() != 0.f ? 1.f / _World.GetDeltaSeconds() : 0.f);

    KL::Debug::ImGui::MainWindow::Delegate::OnDrawBottomBarDelegate.Broadcast(_World);

    ImGui::EndMenuBar();
    ImGui::End();
}

void FKLDebugImGuiWindow::DrawImGuiBar() const
{
    const ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings;
    if(!ImGui::Begin("MainWindow", nullptr, WindowFlags))
    {
        return;
    }

    if (ImGui::BeginTabBar("Selection", ImGuiTabBarFlags_::ImGuiTabBarFlags_None))
    {
        DrawImGuiEngine();

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void FKLDebugImGuiWindow::DrawImGuiEngine() const
{
    //if (ImGui::BeginTabItem("Engine"))
    //{
    //    ImGui::EndTabItem();
    //}
}

#if IMGUI_UNREAL_COMMAND_ENABLED

void FKLDebugImGuiWindow::DrawCommandsMenu() const
{
    if (!ImGui::BeginMenu("Commands"))
    {
        return;
    }

    ImGui::MenuItem("Unreal Command", nullptr, &ImUnrealCommand::IsVisible(KL::Debug::ImGuiEditor::MainWindow::mUnrealCommandContext));
    ImGui::EndMenu();
}

void FKLDebugImGuiWindow::DrawCommands() const
{
    if (ImUnrealCommand::IsVisible(KL::Debug::ImGuiEditor::MainWindow::mUnrealCommandContext))
    {
        ImUnrealCommand::Show(KL::Debug::ImGuiEditor::MainWindow::mUnrealCommandContext);
    }
}

#endif