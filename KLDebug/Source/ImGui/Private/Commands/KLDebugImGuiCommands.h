#pragma once

//engine
#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Templates/SharedPointer.h"

class FUICommandInfo;

class KLDEBUGIMGUI_API FKLDebugImGuiCommands final : public TCommands<FKLDebugImGuiCommands>
{
public:
    FKLDebugImGuiCommands();

	void RegisterCommands() final;

	UE_NODISCARD const TSharedPtr<FUICommandInfo>& GetEnableImGuiCmd() const;

private:
	TSharedPtr<FUICommandInfo> mEnableImGui;
};

inline const TSharedPtr<FUICommandInfo>& FKLDebugImGuiCommands::GetEnableImGuiCmd() const
{
    return mEnableImGui;
}
