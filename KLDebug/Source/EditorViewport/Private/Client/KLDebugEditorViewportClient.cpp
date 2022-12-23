#include "Client/KLDebugEditorViewportClient.h"

FKLDebugEditorViewportClient::FKLDebugEditorViewportClient()
    : FLevelEditorViewportClient(nullptr)
{
    bDrawAxes   = false;
    bIsRealtime = true;
    SetAllowCinematicControl(true);
    bDisableInput = false;
}

void FKLDebugEditorViewportClient::SetViewportWidget(const TSharedPtr<SEditorViewport>& _InViewportWidget)
{
    EditorViewportWidget = _InViewportWidget;
}