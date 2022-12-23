//#include "Layout/KLDebugEditorViewportLayoutEntity.h"
//
//#include "Slate/SKLDebugEditorViewport.h"
//
////engine
//#include "AssetEditorViewportLayout.h"
//#include "CoreMinimal.h"
//#include "Editor/UnrealEdEngine.h"
//#include "Framework/Application/SlateApplication.h"
//#include "ILevelEditor.h"
//#include "SLevelViewport.h"
//#include "UnrealEdGlobals.h"
//#include "Widgets/SWidget.h"
//#include "Widgets/DeclarativeSyntaxSupport.h"
//
//FKLDebugEditorViewportLayoutEntity::FKLDebugEditorViewportLayoutEntity(const FAssetEditorViewportConstructionArgs& _Args, TSharedPtr<ILevelEditor> _InLevelEditor)
//	: Widget(
//        SNew(SKLDebugEditorViewport)
//		.ParentLayout(_Args.ParentLayout)
//		.ParentLevelEditor(_InLevelEditor)
//		.LayoutName(_Args.ConfigKey)
//		)
//{
//}
//
//TSharedRef<SWidget> FKLDebugEditorViewportLayoutEntity::AsWidget() const
//{
//    return Widget;
//}
//
//void FKLDebugEditorViewportLayoutEntity::OnLayoutDestroyed()
//{
//	if (IsPlayInEditorViewportActive() || GetLevelViewportClient().IsSimulateInEditorViewport() )
//	{
//		GUnrealEd->EndPlayMap();
//	}
//}
//
//TSharedPtr<SLevelViewport> FKLDebugEditorViewportLayoutEntity::AsLevelViewport() const
//{
//    return Widget->GetLevelViewport();
//}
//
//FName FKLDebugEditorViewportLayoutEntity::GetType() const
//{
//    static FName DebigViewport("KLDebug");
//    return DebigViewport;
//}
//
//FLevelEditorViewportClient& FKLDebugEditorViewportLayoutEntity::GetLevelViewportClient() const
//{
//    return Widget->GetLevelViewport()->GetLevelViewportClient();
//}
//
//bool FKLDebugEditorViewportLayoutEntity::IsPlayInEditorViewportActive() const
//{
//    return Widget->GetLevelViewport()->IsPlayInEditorViewportActive();
//}
//
//void FKLDebugEditorViewportLayoutEntity::RegisterGameViewportIfPIE()
//{
//    return Widget->GetLevelViewport()->RegisterGameViewportIfPIE();
//}
//
//void FKLDebugEditorViewportLayoutEntity::SetKeyboardFocus()
//{
//    FSlateApplication::Get().SetKeyboardFocus(Widget->GetLevelViewport());
//}
//
//void FKLDebugEditorViewportLayoutEntity::SaveConfig(const FString& _ConfigString)
//{
//    return Widget->GetLevelViewport()->SaveConfig(_ConfigString);
//}