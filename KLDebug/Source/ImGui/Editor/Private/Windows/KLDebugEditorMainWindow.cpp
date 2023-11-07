#include "Windows/KLDebugEditorMainWindow.h"

#include "Examples/NetImguiExample.h"
#include "Examples/NetImguiDemoNodeExample.h"

// modules
#include "ImGui/Framework/Public/Window/KLDebugImGuiWindowDelegates.h"
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"
#include "ThirdParty/ImPlotThirdParty/Public/Library/implot.h"
#include "ThirdParty/NetImGuiThirdParty/Public/NetImGuiThirdPartyModule.h"

// engine
#include "Camera/PlayerCameraManager.h"
#include "Editor/EditorEngine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "LevelEditorViewport.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Subsystems/UnrealEditorSubsystem.h"

namespace KL::Debug::ImGuiEditor::MainWindow
{
    static bool ShowImGuiExample    = false;
    static bool ShowNetImGuiExample = false;
    static bool ShowImPlotExample   = false;
    static bool ShowImNodeGraphExample = false;
}  // namespace KL::Debug::ImGuiEditor::MainWindow

/////////////////////////////////////////////////////////////////////////////////

void FKLDebugEditorMainWindow::Init()
{
    mDrawBottomBarHandle = KL::Debug::ImGui::MainWindow::Delegate::OnDrawBottomBarDelegate.AddRaw(this, &FKLDebugEditorMainWindow::OnDrawBottomBar);
    mDrawTopBarHandle    = KL::Debug::ImGui::MainWindow::Delegate::OnDrawTopBarDelegate.AddRaw(this, &FKLDebugEditorMainWindow::OnDrawTopBar);
}

void FKLDebugEditorMainWindow::Shutdown()
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

void FKLDebugEditorMainWindow::OnDrawTopBar(const UWorld& _World) const
{
    DrawExamplesMenu();

    DrawExamples();
}

void FKLDebugEditorMainWindow::OnDrawBottomBar(const UWorld& _World) const
{
    const APlayerCameraManager* PlayerCamera = UGameplayStatics::GetPlayerCameraManager(&_World, 0);
    if (!PlayerCamera)
    {
        return;
    }

    FVector CameraLocation;
    FRotator CamRot;
    PlayerCamera->GetCameraViewPoint(CameraLocation, CamRot);

    ImGui::SameLine();
    ImGui::Text(" Camera: [%ls]", *CameraLocation.ToString());

  /*  UUnrealEditorSubsystem* UnrealEditorSubsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
    if (!UnrealEditorSubsystem)
    {
        return;
    }

    if (!IsLevelEditorCurrentlySelected(_World))
    {
        return;
    }

    FVector  Pos;
    FRotator Rotator;
    UnrealEditorSubsystem->GetLevelViewportCameraInfo(Pos, Rotator);

    ImGui::SameLine();
    ImGui::Text(" Camera: [%ls]", *Pos.ToString());*/
}

bool FKLDebugEditorMainWindow::IsLevelEditorCurrentlySelected(const UWorld& _World) const
{
    const TArray<FLevelEditorViewportClient*>& ViewportVlients = GEditor->GetLevelViewportClients();
    for (const FLevelEditorViewportClient* LevelVC : ViewportVlients)
    {
        if (!LevelVC || LevelVC->GetWorld() != &_World)
        {
            continue;
        }

        return true;
    }

    return false;
}

void FKLDebugEditorMainWindow::DrawExamplesMenu() const
{
    if (!ImGui::BeginMenu("Examples"))
    {
        return;
    }

    ImGui::MenuItem("DearImGui", nullptr, &KL::Debug::ImGuiEditor::MainWindow::ShowImGuiExample);
    ImGui::MenuItem("NetImGui", nullptr, &KL::Debug::ImGuiEditor::MainWindow::ShowNetImGuiExample);
    ImGui::MenuItem("ImPlot", nullptr, &KL::Debug::ImGuiEditor::MainWindow::ShowImPlotExample);
    ImGui::MenuItem("NodeGraph", nullptr, &KL::Debug::ImGuiEditor::MainWindow::ShowImNodeGraphExample);

    ImGui::EndMenu();
}

void FKLDebugEditorMainWindow::DrawExamples() const
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

    if (KL::Debug::ImGuiEditor::MainWindow::ShowImNodeGraphExample)
    {
        NodeEditorDemo::ShowDemo(KL::Debug::ImGuiEditor::MainWindow::ShowImNodeGraphExample);
    }
}
