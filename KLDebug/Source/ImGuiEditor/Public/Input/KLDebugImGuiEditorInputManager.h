#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "Templates/UnrealTemplate.h"

class FUICommandList;

class KLDEBUGIMGUIEDITOR_API FKLDebugImGuiEditorInputManager final : public FNoncopyable
{
public:
    void Init();
    void Shutdown();

private:
    void OnEnableImGui();

private:
    TSharedPtr<FUICommandList> CommandList;
    bool                       mIsImGuiEnable = false;
};
