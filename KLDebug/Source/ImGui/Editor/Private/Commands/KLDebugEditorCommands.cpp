#include "Commands/KLDebugEditorCommands.h"

// #include "Config/KLDebugImGuiEditorConfig.h"
// #include "Input/KLDebugImGuiEditorInputConfig.h"

// engine
#include "Framework/Commands/InputChord.h"
#include "Framework/Commands/UICommandInfo.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "KLDebugImGuiCommands"

FKLDebugEditorCommands::FKLDebugEditorCommands()
    : TCommands<FKLDebugEditorCommands>("KLDebugEditorCommands", LOCTEXT("KLDebugEditorCommands", "KL Debug Commands"), NAME_None, FAppStyle::GetAppStyleSetName())
{
}

void FKLDebugEditorCommands::RegisterCommands()
{
    // const UKLDebugImGuiEditorConfig& Config = UKLDebugImGuiEditorConfig::Get();
    // const FKLDebugImGuiEditorInputConfig& InputConfig = Config.GetInputConfig();

    // UI_COMMAND(mEnableImGui, "EnableImGuiViewport", "Enable ImGui Viewport", EUserInterfaceActionType::Button, InputConfig.GetEnableViewportKey());
}

#undef LOCTEXT_NAMESPACE
