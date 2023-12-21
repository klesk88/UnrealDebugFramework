// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Extender/KLDebugEditorMenuExtender_Style.h"

// engine
#include "Interfaces/IPluginManager.h"
#include "Math/Vector2D.h"
#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleRegistry.h"

TSharedPtr<FSlateStyleSet> FKLDebugEditorMenuExtender_Style::mStyleInstance = nullptr;

void FKLDebugEditorMenuExtender_Style::Init()
{
    if (!mStyleInstance.IsValid())
    {
        mStyleInstance = Create();
        FSlateStyleRegistry::RegisterSlateStyle(*mStyleInstance);
        ReloadTextures();
    }
}

void FKLDebugEditorMenuExtender_Style::Shutdown()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(*mStyleInstance);
    ensure(mStyleInstance.IsUnique());
    mStyleInstance.Reset();
}

const FName& FKLDebugEditorMenuExtender_Style::GetStyleSetName()
{
    static FName StyleSetName(TEXT("KLDebugEditorMenuExtender_Style"));
    return StyleSetName;
}

const FName& FKLDebugEditorMenuExtender_Style::GetIconName()
{
    static FName IconName(TEXT("KLDebug.EditorIcon"));
    return IconName;
}

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedRef<FSlateStyleSet> FKLDebugEditorMenuExtender_Style::Create()
{
    static const FVector2D Icon40x40(40.0f, 40.0f);

    TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
    Style->SetContentRoot(IPluginManager::Get().FindPlugin("KLDebug")->GetBaseDir() / TEXT("Resources"));

    Style->Set(GetIconName(), new IMAGE_BRUSH(TEXT("KLDebug_Icon40"), Icon40x40));

    return Style;
}

#undef IMAGE_BRUSH

void FKLDebugEditorMenuExtender_Style::ReloadTextures()
{
    if (FSlateApplication::IsInitialized())
    {
        FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
    }
}

const ISlateStyle& FKLDebugEditorMenuExtender_Style::Get()
{
    return *mStyleInstance;
}
