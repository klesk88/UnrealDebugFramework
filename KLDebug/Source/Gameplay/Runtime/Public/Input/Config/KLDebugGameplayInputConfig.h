#pragma once

// engine
#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "UObject/Class.h"
#include "UObject/SoftObjectPtr.h"

#include "KLDebugGameplayInputConfig.generated.h"

USTRUCT()
struct KLDEBUGGAMEPLAYRUNTIME_API FKLDebugGameplayInputConfig
{
    GENERATED_BODY()

public:
    void Load() const;

    UE_NODISCARD const UInputMappingContext* TryGetMappingContext() const;
    UE_NODISCARD int32                       GetInputContextPriority() const;
    UE_NODISCARD const UInputAction*         TryGetToogleDebug() const;
    UE_NODISCARD const UInputAction*         TryGetToogleImGuiInput() const;
    UE_NODISCARD const UInputAction*         TryGetToogleDebugCamera() const;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Enanched Input")
    TSoftObjectPtr<UInputMappingContext> MappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Enanched Input")
    int32 InputContextPriority = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Enanched Input")
    TSoftObjectPtr<UInputAction> ToogleDebug;

    UPROPERTY(EditDefaultsOnly, Category = "Enanched Input")
    TSoftObjectPtr<UInputAction> ToogleInput;
    
    UPROPERTY(EditDefaultsOnly, Category = "Enanched Input")
    TSoftObjectPtr<UInputAction> ToogleDebugCamera;

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

inline const UInputAction* FKLDebugGameplayInputConfig::TryGetToogleImGuiInput() const
{
    return ToogleInput.Get();
}

inline const UInputAction* FKLDebugGameplayInputConfig::TryGetToogleDebugCamera() const
{
    return ToogleDebugCamera.Get();
}
