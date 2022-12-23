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
}
