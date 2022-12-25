#pragma once

// engine
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "Templates/UnrealTemplate.h"

class _World;

class KLDEBUGIMGUIEDITOR_API FKLDebugImGuiEditorMainWindow final : public FNoncopyable
{
public:
    void Init();
    void Shutdown();

private:
    void OnDrawTopBar(const UWorld& _World) const;
    void OnDrawBottomBar(const UWorld& _World) const;

    //return true if the currently selected client viewport is the editor one. This can happen in 2 cases:
    // when not playing yet and just be in editor mode
    // after pressing play and be in PIE and press F8 to detach the camera from the player
    UE_NODISCARD bool IsLevelEditorCurrentlySelected(const UWorld& _World) const;

    void DrawExamplesMenu() const;
    void DrawExamples() const;

private:
    FDelegateHandle mDrawBottomBarHandle;
    FDelegateHandle mDrawTopBarHandle;
};
