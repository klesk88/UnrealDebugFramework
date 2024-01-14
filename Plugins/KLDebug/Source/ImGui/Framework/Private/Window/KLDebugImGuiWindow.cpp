// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Window/KLDebugImGuiWindow.h"

#include "BottomBar/Manager/KLDebugFrameworkBottomBarManager.h"
#include "Commands/ImUnrealCommand.h"

// modules
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui_internal.h"    // https://github.com/ocornut/imgui/issues/3518
#include "User/Framework/Public/Window/KLDebugWindowDelegates.h"

// engine
#include "Camera/PlayerCameraManager.h"
#include "Engine/DebugCameraController.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

namespace KL::Debug::ImGuiEditor::MainWindow
{
#if IMGUI_UNREAL_COMMAND_ENABLED
    static ImUnrealCommand::CommandContext* mUnrealCommandContext = nullptr;
#endif
}    // namespace KL::Debug::ImGuiEditor::MainWindow

void FKLDebugImGuiWindow::Init()
{
#if IMGUI_UNREAL_COMMAND_ENABLED
    KL::Debug::ImGuiEditor::MainWindow::mUnrealCommandContext = ImUnrealCommand::Create();    // Create a new Imgui Command Window

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

void FKLDebugImGuiWindow::Update(const UWorld& _World, FKLDebugFrameworkBottomBarManager& _BottomBarManager)
{
    DrawImGuiTopBar(_World, _BottomBarManager);
    DrawImGuiBottomBar(_World, _BottomBarManager);

#if IMGUI_UNREAL_COMMAND_ENABLED
    DrawCommands();
#endif
}

void FKLDebugImGuiWindow::DrawImGuiTopBar(const UWorld& _World, FKLDebugFrameworkBottomBarManager& _BottomBarManager) const
{
    if (!ImGui::BeginMainMenuBar())
    {
        return;
    }

#if IMGUI_UNREAL_COMMAND_ENABLED
    DrawCommandsMenu();
#endif

    DrawImGuiBottomBarsSelection(_World, _BottomBarManager);

    KL::Debug::ImGui::MainWindow::Delegate::OnDrawTopBarDelegate.Broadcast(_World);

    ImGui::EndMainMenuBar();
}

void FKLDebugImGuiWindow::DrawImGuiBottomBar(const UWorld& _World, const FKLDebugFrameworkBottomBarManager& _BottomBarManager) const
{
    ImGuiViewportP* Viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
    const ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    const float Height = ImGui::GetFrameHeight();

    // FIXME: Need to fix when we resize the editor viewport after pressing f8. In that case it seems to don't
    // scale for some reason
    if (!ImGui::BeginViewportSideBar("##SecondaryMenuBar", Viewport, ImGuiDir_Down, Height, WindowFlags))
    {
        return;
    }

    if (!ImGui::BeginMenuBar())
    {
        ImGui::End();
        return;
    }

    _BottomBarManager.DrawBottomBar(_World);
    ImGui::EndMenuBar();
    ImGui::End();
}

void FKLDebugImGuiWindow::DrawImGuiBottomBarsSelection(const UWorld& _World, FKLDebugFrameworkBottomBarManager& _BottomBarManager) const
{
    if (!ImGui::BeginMenu("Tools"))
    {
        return;
    }

    if (ImGui::BeginMenu("BottomBars"))
    {
        int32 IndexSelected = -1;
        for (int32 i = 0; i < _BottomBarManager.GetSortedBars().Num(); i++)
        {
            const FKLDebugFrameworkBottomBarSortedData& BarID = _BottomBarManager.GetSortedBars()[i];
            if (ImGui::MenuItem(TCHAR_TO_ANSI(*BarID.GetName())))
            {
                IndexSelected = i;
            }
        }

        if (IndexSelected != -1)
        {
            _BottomBarManager.UpdateBottomBarIfNeeded(_World, IndexSelected);
        }

        ImGui::EndMenu();
    }

    ImGui::EndMenu();
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