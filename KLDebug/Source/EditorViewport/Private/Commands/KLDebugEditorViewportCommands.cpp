#include "Commands/KLDebugEditorViewportCommands.h"

//engine
#include "Framework/Commands/InputChord.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "DebugEditorViewportCommands"

FKLDebugEditorViewportCommands::FKLDebugEditorViewportCommands()
    : TCommands<FKLDebugEditorViewportCommands>("KLDebugEditor", LOCTEXT("KLDebugEditor", "KLDebugEditor"), NAME_None, FAppStyle::GetAppStyleSetName())
{
}

void FKLDebugEditorViewportCommands::RegisterCommands()
{
	UI_COMMAND(mToggleDebugViewportCommand, "Cinematic Viewport", "A viewport layout tailored to cinematic preview", EUserInterfaceActionType::RadioButton, FInputChord());
}

#undef LOCTEXT_NAMESPACE
