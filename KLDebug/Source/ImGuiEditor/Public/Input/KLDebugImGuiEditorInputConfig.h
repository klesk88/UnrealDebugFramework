#pragma once

// engine
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Framework/Commands/InputChord.h"

#include "KLDebugImGuiEditorInputConfig.generated.h"

USTRUCT()
struct KLDEBUGIMGUIEDITOR_API FKLDebugImGuiEditorInputConfig
{
    GENERATED_BODY()

public:
    UE_NODISCARD const FInputChord& GetEnableViewportKey() const;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    FInputChord EnableViewportKey{EKeys::D, EModifierKey::Alt};
};

inline const FInputChord& FKLDebugImGuiEditorInputConfig::GetEnableViewportKey() const
{
    return EnableViewportKey;
}