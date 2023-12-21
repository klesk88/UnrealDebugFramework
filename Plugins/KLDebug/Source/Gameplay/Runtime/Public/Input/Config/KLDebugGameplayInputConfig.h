// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "UObject/Class.h"
#include "UObject/SoftObjectPtr.h"

#include "KLDebugGameplayInputConfig.generated.h"

UENUM()
enum class EKLDebugGameplayInputType : uint8
{
    ToogleDebug = 0,
    ToogleInput,
    ToogleCamera,
    TooglePause,

    Count
};

USTRUCT()
struct KLDEBUGGAMEPLAYRUNTIME_API FKLDebugGameplayInputConfig
{
    GENERATED_BODY()

public:
    void Load() const;

    UE_NODISCARD const UInputMappingContext* TryGetMappingContext() const;
    UE_NODISCARD int32 GetInputContextPriority() const;

    UE_NODISCARD const UInputAction* TryGetInputAction(const EKLDebugGameplayInputType& _Action) const;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Enanched Input")
    TSoftObjectPtr<UInputMappingContext> MappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Enanched Input")
    int32 InputContextPriority = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Enanched Input")
    TSoftObjectPtr<UInputAction> InputActions[static_cast<int32>(EKLDebugGameplayInputType::Count)];
};

inline const UInputMappingContext* FKLDebugGameplayInputConfig::TryGetMappingContext() const
{
    return MappingContext.Get();
}

inline int32 FKLDebugGameplayInputConfig::GetInputContextPriority() const
{
    return InputContextPriority;
}

inline const UInputAction* FKLDebugGameplayInputConfig::TryGetInputAction(const EKLDebugGameplayInputType& _Action) const
{
    check(_Action != EKLDebugGameplayInputType::Count);
    return InputActions[static_cast<int32>(_Action)].Get();
}
