#pragma once

// engine
#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Templates/SharedPointer.h"

class FUICommandInfo;

class KLDEBUGIMGUIEDITOR_API FKLDebugEditorCommands final : public TCommands<FKLDebugEditorCommands>
{
public:
    FKLDebugEditorCommands();

    void RegisterCommands() final;

    UE_NODISCARD const TSharedPtr<FUICommandInfo>& GetEnableImGuiCmd() const;

private:
    TSharedPtr<FUICommandInfo> mEnableImGui;
};

inline const TSharedPtr<FUICommandInfo>& FKLDebugEditorCommands::GetEnableImGuiCmd() const
{
    return mEnableImGui;
}
