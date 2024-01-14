// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Public/Context/KLDebugContextInterface.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugFrameworkBottomBarManager;
class FKLDebugFrameworkModeManager;
class UWorld;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiWindow final : public FNoncopyable
{
public:
    void Init();
    void Shutdown();
    void Update(const UWorld& _World, FKLDebugFrameworkBottomBarManager& _BottomBarManager, FKLDebugFrameworkModeManager& _ModeManager);

    UE_NODISCARD int32 GetCurrentModeIndex() const;
    UE_NODISCARD IKLDebugContextInterface* GetCurrentModeContext() const;

private:
    void DrawImGuiTopBar(const UWorld& _World, FKLDebugFrameworkBottomBarManager& _BottomBarManager, FKLDebugFrameworkModeManager& _ModeManager);
    void DrawImGuiBottomBar(const UWorld& _World, const FKLDebugFrameworkBottomBarManager& _BottomBarManager) const;

    void DrawImGuiBottomBarsSelection(const UWorld& _World, FKLDebugFrameworkBottomBarManager& _BottomBarManager);
    void DrawImGuiModeSelection(const UWorld& _World, FKLDebugFrameworkModeManager& _ModeManager);

private:
    int32 mBottomBarInterface = -1;
    TUniquePtr<IKLDebugContextInterface> mBottomBarContext;
    int32 mModeInterface = -1;
    TUniquePtr<IKLDebugContextInterface> mModeContext;

#if IMGUI_UNREAL_COMMAND_ENABLED
private:
    void DrawCommandsMenu() const;
    void DrawCommands() const;
#endif
};

inline int32 FKLDebugImGuiWindow::GetCurrentModeIndex() const
{
    return mModeInterface;
}

inline IKLDebugContextInterface* FKLDebugImGuiWindow::GetCurrentModeContext() const
{
    return mModeContext.Get();
}