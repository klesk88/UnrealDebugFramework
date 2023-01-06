#include "Input/KLDebugImGuiEditorInputManager.h"

#include "Commands/KLDebugImGuiCommands.h"

// KLImGui module
#include "ImGui/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"

//engine
#include "Framework/Commands/UIAction.h"
#include "Framework/Commands/UICommandList.h"
#include "Kismet2/DebuggerCommands.h"

void FKLDebugImGuiEditorInputManager::Init()
{
    FKLDebugImGuiCommands::Register();

    CommandList = FPlayWorldCommands::GlobalPlayWorldActions;
    CommandList->MapAction(FKLDebugImGuiCommands::Get().GetEnableImGuiCmd(),
                           FExecuteAction::CreateRaw(this, &FKLDebugImGuiEditorInputManager::OnEnableImGui));
}

void FKLDebugImGuiEditorInputManager::Shutdown()
{
    FKLDebugImGuiCommands::Unregister();

    CommandList = nullptr;
}

void FKLDebugImGuiEditorInputManager::OnEnableImGui()
{
    UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
    if (!ImGuiEngineSubsystem)
    {
        return;
    }

   // ImGuiEngineSubsystem->ToogleImGuiSystemState();
}
