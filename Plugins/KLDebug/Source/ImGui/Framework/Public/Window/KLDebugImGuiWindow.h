// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Templates/UnrealTemplate.h"

class FKLDebugFrameworkBottomBarManager;
class UWorld;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiWindow final : public FNoncopyable
{
public:
    void Init();
    void Shutdown();
    void Update(const UWorld& _World, FKLDebugFrameworkBottomBarManager& _BottomBarManager);

private:
    void DrawImGuiTopBar(const UWorld& _World, FKLDebugFrameworkBottomBarManager& _BottomBarManager) const;
    void DrawImGuiBottomBar(const UWorld& _World, const FKLDebugFrameworkBottomBarManager& _BottomBarManager) const;

    void DrawImGuiBottomBarsSelection(const UWorld& _World, FKLDebugFrameworkBottomBarManager& _BottomBarManager) const;

#if IMGUI_UNREAL_COMMAND_ENABLED
private:
    void DrawCommandsMenu() const;
    void DrawCommands() const;
#endif
};
