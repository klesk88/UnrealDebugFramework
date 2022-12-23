#pragma once

//engine
#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UICommandInfo.h"
#include "Templates/SharedPointer.h"

class KLDEBUGEDITORVIEWPORT_API FKLDebugEditorViewportCommands final : public TCommands<FKLDebugEditorViewportCommands>
{
public:
	/** Default constructor. */
    FKLDebugEditorViewportCommands();

	/** Initialize commands */
	void RegisterCommands() final;

	UE_NODISCARD const TSharedPtr<FUICommandInfo>& GetViewportCommand() const;

private:
	TSharedPtr<FUICommandInfo> mToggleDebugViewportCommand;
};

inline const TSharedPtr<FUICommandInfo>& FKLDebugEditorViewportCommands::GetViewportCommand() const
{
    return mToggleDebugViewportCommand;
}