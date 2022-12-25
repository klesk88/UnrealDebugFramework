#include "Windows/KLDebugImGuiEditorMainWindow.h"

// ImGuiThirdParty module
#include "ImGuiThirdParty/Public/Library/imgui.h"
// KLImGui module
#include "ImGui/Public/Window/KLDebugImGuiWindowDelegates.h"

//engine
#include "Editor/EditorEngine.h"
#include "Engine/World.h"
#include "LevelEditorViewport.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Subsystems/UnrealEditorSubsystem.h"

void FKLDebugImGuiEditorMainWindow::Init()
{
    mDrawBottomBarHandle = KL::Debug::ImGui::MainWindow::Delegate::OnDrawBottomBarDelegate.AddRaw(this, &FKLDebugImGuiEditorMainWindow::OnDrawBottomBar);
}

void FKLDebugImGuiEditorMainWindow::Shutdown()
{
    if (mDrawBottomBarHandle.IsValid())
    {
        KL::Debug::ImGui::MainWindow::Delegate::OnDrawBottomBarDelegate.Remove(mDrawBottomBarHandle);
        mDrawBottomBarHandle.Reset();
    }
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
