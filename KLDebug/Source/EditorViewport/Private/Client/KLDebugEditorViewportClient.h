#pragma once

// engine
#include "CoreMinimal.h"
#include "LevelEditorViewport.h"
#include "Templates/SharedPointer.h"

class SEditorViewport;

/** Overridden level viewport client for this viewport */
class FKLDebugEditorViewportClient : FLevelEditorViewportClient
{
    FKLDebugEditorViewportClient();

    void SetViewportWidget(const TSharedPtr<SEditorViewport>& _InViewportWidget);
};
