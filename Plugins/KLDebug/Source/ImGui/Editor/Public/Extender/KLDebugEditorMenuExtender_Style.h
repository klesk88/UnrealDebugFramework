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

    [[nodiscard]] static const ISlateStyle& Get();
    [[nodiscard]] static const FName& GetStyleSetName();
    [[nodiscard]] static const FName& GetIconName();

private:
    static void ReloadTextures();
    [[nodiscard]] static TSharedRef<FSlateStyleSet> Create();

private:
    static TSharedPtr<FSlateStyleSet> mStyleInstance;
};