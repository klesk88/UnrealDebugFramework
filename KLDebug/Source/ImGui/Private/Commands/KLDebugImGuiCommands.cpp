#include "Commands/KLDebugImGuiCommands.h"

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
    UI_COMMAND(mEnableImGui, "EnableImGuiViewport", "Enable ImGui Viewport", EUserInterfaceActionType::Button, FInputChord(EKeys::D, EModifierKey::Alt));
}

#undef LOCTEXT_NAMESPACE
