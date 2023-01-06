#pragma once

#include "Input/Config/KLDebugGameplayInputConfig.h"

// engine
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InstancedStruct.h"

#include "KLDebugGameplayConfig.generated.h"

UCLASS(config = Plugins, defaultconfig, DisplayName = "KLImGui")
class KLDEBUGGAMEPLAY_API UKLDebugGameplayConfig final : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UKLDebugGameplayConfig();

    UE_NODISCARD static const UKLDebugGameplayConfig& Get();

    UE_NODISCARD const FKLDebugGameplayInputConfig& GetInputConfig() const;

private:
    UPROPERTY(EditDefaultsOnly, config)
    FKLDebugGameplayInputConfig InputConfig;
};

inline const UKLDebugGameplayConfig& UKLDebugGameplayConfig::Get()
{
    const UKLDebugGameplayConfig* Config = GetDefault<UKLDebugGameplayConfig>();
    checkf(Config != nullptr, TEXT("config should be valid"));
    return *Config;
}

inline const FKLDebugGameplayInputConfig& UKLDebugGameplayConfig::GetInputConfig() const
{
    return InputConfig;
}