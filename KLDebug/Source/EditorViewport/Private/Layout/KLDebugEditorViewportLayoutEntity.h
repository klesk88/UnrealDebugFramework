//#pragma once
//
////engine
//#include "Containers/UnrealString.h"
//#include "CoreMinimal.h"
//#include "LevelViewportLayout.h"
//#include "Templates/SharedPointer.h"
//#include "UObject/NameTypes.h"
//
//class FLevelEditorViewportClient;
//class ILevelEditor;
//class SLevelViewport;
//class SKLDebugEditorViewport;
//class SWidget;
//
//struct FAssetEditorViewportConstructionArgs;
//
//class FKLDebugEditorViewportLayoutEntity final : public ILevelViewportLayoutEntity
//{
//public:
//    FKLDebugEditorViewportLayoutEntity(const FAssetEditorViewportConstructionArgs& _Args, TSharedPtr<ILevelEditor> _InLevelEditor);
//
//	TSharedRef<SWidget>        AsWidget() const override;
//    TSharedPtr<SLevelViewport> AsLevelViewport() const override;
//
//	FName GetType() const override;
//
//	FLevelEditorViewportClient& GetLevelViewportClient() const override;
//    bool                        IsPlayInEditorViewportActive() const override;
//    void                        RegisterGameViewportIfPIE() override;
//    void                        SetKeyboardFocus() override;
//    void                        OnLayoutDestroyed() override;
//    void                        SaveConfig(const FString& _ConfigString) override;
//
//private:
//	/** This entity's widget */
//    TSharedRef<SKLDebugEditorViewport> Widget;
//};
