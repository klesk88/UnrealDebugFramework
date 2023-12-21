// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"
#include "Templates/SharedPointer.h"
#include "UObject/NameTypes.h"

class FSlateStyleSet;
class ISlateStyle;

/*
 * based on code from github project of nakdeyes/UnrealImGuiTools
 * this allow us to add a small png to the button we create
 */
class KLDEBUGIMGUIEDITOR_API FKLDebugEditorMenuExtender_Style
{
public:
    static void Init();
    static void Shutdown();

    UE_NODISCARD static const ISlateStyle& Get();
    UE_NODISCARD static const FName& GetStyleSetName();
    UE_NODISCARD static const FName& GetIconName();

private:
    static void ReloadTextures();
    UE_NODISCARD static TSharedRef<FSlateStyleSet> Create();

private:
    static TSharedPtr<FSlateStyleSet> mStyleInstance;
};