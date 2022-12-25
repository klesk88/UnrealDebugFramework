#include "Windows/KLDebugImGuiEditorMainWindow.h"

#include "Examples/NetImguiExample.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"
// ImPlotThirdParty module
#include "ImPlotThirdParty/Public/Library/implot.h"
// KLImGui module
#include "ImGui/Public/Window/KLDebugImGuiWindowDelegates.h"
//NetImGuiThirdParty module
#include "NetImGuiThirdParty/Public/NetImGuiThirdPartyModule.h"

//engine
#include "Editor/EditorEngine.h"
#include "Engine/World.h"
#include "LevelEditorViewport.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Subsystems/UnrealEditorSubsystem.h"

namespace KL::Debug::ImGuiEditor::MainWindow
{
    static bool ShowImGuiExample    = false;
    static bool ShowNetImGuiExample = false;
    static bool ShowImPlotExample   = false;
}

/////////////////////////////////////////////////////////////////////////////////

void FKLDebugImGuiEditorMainWindow::Init()
{
    mDrawBottomBarHandle = KL::Debug::ImGui::MainWindow::Delegate::OnDrawBottomBarDelegate.AddRaw(this, &FKLDebugImGuiEditorMainWindow::OnDrawBottomBar);
    mDrawTopBarHandle    = KL::Debug::ImGui::MainWindow::Delegate::OnDrawTopBarDelegate.AddRaw(this, &FKLDebugImGuiEditorMainWindow::OnDrawTopBar);
}

void FKLDebugImGuiEditorMainWindow::Shutdown()
{
    if (mDrawBottomBarHandle.IsValid())
    {
        KL::Debug::ImGui::MainWindow::Delegate::OnDrawBottomBarDelegate.Remove(mDrawBottomBarHandle);
        mDrawBottomBarHandle.Reset();
    }

    if (mDrawTopBarHandle.IsValid())
    {
        KL::Debug::ImGui::MainWindow::Delegate::OnDrawBottomBarDelegate.Remove(mDrawBottomBarHandle);
        mDrawTopBarHandle.Reset();
    }
}

void FKLDebugImGuiEditorMainWindow::OnDrawTopBar(const UWorld& _World) const
{
    DrawExamplesMenu();

    DrawExamples();
}

void FKLDebugImGuiEditorMainWindow::OnDrawBottomBar(const UWorld& _World) const
{
    UUnrealEditorSubsystem* UnrealEditorSubsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
    if (!UnrealEditorSubsystem)
    {
        return;
    }

    if (!IsLevelEditorCurrentlySelected(_World))
    {
        return;
    }

    FVector Pos;
    FRotator Rotator;
    UnrealEditorSubsystem->GetLevelViewportCameraInfo(Pos, Rotator);

    ImGui::SameLine();
    ImGui::Text(" Camera: [%ls]", *Pos.ToString());
}

bool FKLDebugImGuiEditorMainWindow::IsLevelEditorCurrentlySelected(const UWorld& _World) const
{
    for (const FLevelEditorViewportClient* LevelVC : GEditor->GetLevelViewportClients())
    {
        if (!LevelVC || LevelVC->GetWorld() != &_World)
        {
            continue;
        }

        return true;
    }

    return false;
}

void FKLDebugImGuiEditorMainWindow::DrawExamplesMenu() const
{
    if (!ImGui::BeginMenu("Examples"))
    {
        return;
    }

    ImGui::MenuItem("DearImGui", nullptr, &KL::Debug::ImGuiEditor::MainWindow::ShowImGuiExample);
    ImGui::MenuItem("NetImGui", nullptr, &KL::Debug::ImGuiEditor::MainWindow::ShowNetImGuiExample);
    ImGui::MenuItem("ImPlot", nullptr, &KL::Debug::ImGuiEditor::MainWindow::ShowImPlotExample);

    ImGui::EndMenu();
}

void FKLDebugImGuiEditorMainWindow::DrawExamples() const
{
    if (KL::Debug::ImGuiEditor::MainWindow::ShowImGuiExample)
    {
        ImGui::ShowDemoWindow(&KL::Debug::ImGuiEditor::MainWindow::ShowImGuiExample);
    }

    if (KL::Debug::ImGuiEditor::MainWindow::ShowNetImGuiExample)
    {
        NetImGui::Demo::DrawDemo(&KL::Debug::ImGuiEditor::MainWindow::ShowNetImGuiExample);
    }

    if (KL::Debug::ImGuiEditor::MainWindow::ShowImPlotExample)
    {
        ImPlot::ShowDemoWindow(&KL::Debug::ImGuiEditor::MainWindow::ShowImPlotExample);
    }
}
