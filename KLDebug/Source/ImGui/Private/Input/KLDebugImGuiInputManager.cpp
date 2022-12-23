#include "Input/KLDebugImGuiInputManager.h"

#include "Commands/KLDebugImGuiCommands.h"

// KLUnrealImGui module
#include "UnrealImGui/Public/KLUnrealImGuiModule.h"

//engine
#include "Framework/Commands/UIAction.h"
#include "Framework/Commands/UICommandList.h"

#if WITH_EDITOR
#include "Kismet2/DebuggerCommands.h"
#endif

void FKLDebugImGuiInputManager::Init()
{
    FKLDebugImGuiCommands::Register();

    CommandList = FPlayWorldCommands::GlobalPlayWorldActions;
    CommandList->MapAction(FKLDebugImGuiCommands::Get().GetEnableImGuiCmd(),
                           FExecuteAction::CreateRaw(this, &FKLDebugImGuiInputManager::OnEnableImGui));
}

void FKLDebugImGuiInputManager::Shutdown()
{
    FKLDebugImGuiCommands::Unregister();

    CommandList = nullptr;
}

void FKLDebugImGuiInputManager::OnEnableImGui()
{
    FKLUnrealImGuiModule& KLUnrealImgui = FKLUnrealImGuiModule::Get();

    if (mIsImGuiEnable)
    {
        KLUnrealImgui.DisableImGuiSystem();
    }
    else
    {
        KLUnrealImgui.EnableImGuiSystem();
    }

    mIsImGuiEnable = !mIsImGuiEnable;
}
