#pragma once

// engine
#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "UObject/Class.h"
#include "UObject/ObjectPtr.h"

#include "KLDebugGameplayInputConfig.generated.h"

USTRUCT()
struct KLDEBUGGAMEPLAY_API FKLDebugGameplayInputConfig
{
    GENERATED_BODY()

public:
    UE_NODISCARD const UInputMappingContext* TryGetMappingContext() const;
    UE_NODISCARD int32                       GetInputContextPriority() const;
    UE_NODISCARD const UInputAction*         TryGetToogleDebug() const;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Enanched Input")
    TObjectPtr<UInputMappingContext> MappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Enanched Input")
    int32 InputContextPriority;

    UPROPERTY(EditDefaultsOnly, Category = "Enanched Input")
    TObjectPtr<UInputAction> ToogleDebug;
};

inline const UInputMappingContext* FKLDebugGameplayInputConfig::TryGetMappingContext() const
{
    return MappingContext.Get();
}

inline int32 FKLDebugGameplayInputConfig::GetInputContextPriority() const
{
    return InputContextPriority;
}

inline const UInputAction* FKLDebugGameplayInputConfig::TryGetToogleDebug() const
{
    return ToogleDebug.Get();
}
