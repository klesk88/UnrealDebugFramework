// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include <Delegates/Delegate.h>

class UWorld;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDrawMainImGuiWindow, const UWorld& /*_World*/);

namespace KL::Debug::ImGui::MainWindow::Delegate
{
    // this is called when drawing the top bar of the Viewport ImGui window
    KLDEBUGUSERFRAMEWORK_API extern FOnDrawMainImGuiWindow OnDrawTopBarDelegate;
}    // namespace KL::Debug::ImGui::MainWindow::Delegate
