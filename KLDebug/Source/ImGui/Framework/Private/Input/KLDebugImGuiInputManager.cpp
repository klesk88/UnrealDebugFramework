#include "Input/KLDebugImGuiInputManager.h"

// KLUnrealImGui module
#include "UnrealImGui/Public/KLUnrealImGuiModule.h"

void FKLDebugImGuiInputManager::Init()
{
}

void FKLDebugImGuiInputManager::Shutdown()
{
}

void FKLDebugImGuiInputManager::ToogleImGuiSystem()
{
    FKLUnrealImGuiModule& KLUnrealImgui = FKLUnrealImGuiModule::Get();

    if (mIsImGuiEnable)
    {
        KLUnrealImgui.DisableImGuiSystem();
    }
    else
    {
        KLUnrealImgui.EnableImGuiSystem();
    }

    mIsImGuiEnable = !mIsImGuiEnable;
    if (!mIsImGuiEnable)
    {
        KLUnrealImgui.GetProperties().SetInputEnabled(false);
        KLUnrealImgui.GetProperties().SetKeyboardInputShared(false);
    }
}

void FKLDebugImGuiInputManager::ToogleImGuiInput()
{
    FKLUnrealImGuiModule& KLUnrealImgui = FKLUnrealImGuiModule::Get();
    KLUnrealImgui.GetProperties().ToggleInput();
    KLUnrealImgui.GetProperties().SetKeyboardInputShared(KLUnrealImgui.GetProperties().IsInputEnabled());
}
