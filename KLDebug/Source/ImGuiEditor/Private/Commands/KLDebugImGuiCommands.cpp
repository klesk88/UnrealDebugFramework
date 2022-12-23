#include "Commands/KLDebugImGuiCommands.h"

#include "Config/KLDebugImGuiEditorConfig.h"
#include "Input/KLDebugImGuiEditorInputConfig.h"

//engine
#include "Framework/Commands/InputChord.h"
#include "Framework/Commands/UICommandInfo.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "KLDebugImGuiCommands"

FKLDebugImGuiCommands::FKLDebugImGuiCommands()
    : TCommands<FKLDebugImGuiCommands>("KLDebugImGuiCommands", LOCTEXT("KLDebugImGuiCommands", "Level Sequence Editor"), NAME_None, FAppStyle::GetAppStyleSetName())
{
}

void FKLDebugImGuiCommands::RegisterCommands()
{
    const UKLDebugImGuiEditorConfig& Config = UKLDebugImGuiEditorConfig::Get();
    const FKLDebugImGuiEditorInputConfig& InputConfig = Config.GetInputConfig();

    UI_COMMAND(mEnableImGui, "EnableImGuiViewport", "Enable ImGui Viewport", EUserInterfaceActionType::Button, InputConfig.GetEnableViewportKey());
}

#undef LOCTEXT_NAMESPACE
