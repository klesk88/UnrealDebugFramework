#pragma once

// engine
#include "CoreMinimal.h"
#include <Delegates/Delegate.h>

class UWorld;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDrawMainImGuiWindow, const UWorld& /*_World*/);

namespace KL::Debug::ImGui::MainWindow::Delegate
{
    //this is called when drawing the top bar of the Viewport ImGui window
    KLDEBUGIMGUIFRAMEWORK_API extern FOnDrawMainImGuiWindow OnDrawTopBarDelegate;
    // this is called when drawing the top bar of the Viewport ImGui window
    KLDEBUGIMGUIFRAMEWORK_API extern FOnDrawMainImGuiWindow OnDrawBottomBarDelegate;
}

