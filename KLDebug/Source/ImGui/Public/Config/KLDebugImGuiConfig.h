#pragma once

// engine
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InstancedStruct.h"

#include "KLDebugImGuiConfig.generated.h"

UCLASS(config=Plugins, defaultconfig, DisplayName = "KLImGui")
class KLDEBUGIMGUI_API UKLDebugImGuiConfig final : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UKLDebugImGuiConfig();

    UE_NODISCARD static const UKLDebugImGuiConfig& Get();

    UE_NODISCARD const FInstancedStruct& GetImGuiWindow() const;

private:
    UPROPERTY(EditDefaultsOnly, config, NoClear, meta = (BaseStruct = "KLDebugImGuiWindow"))
    FInstancedStruct ImGuiWindowManager;
};

inline const UKLDebugImGuiConfig& UKLDebugImGuiConfig::Get()
{
    const UKLDebugImGuiConfig* Config = GetDefault<UKLDebugImGuiConfig>();
    checkf(Config != nullptr, TEXT("config should be valid"));
    return *Config;
}

inline const FInstancedStruct& UKLDebugImGuiConfig::GetImGuiWindow() const
{
    return ImGuiWindowManager;
}