// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Config/KLDebugImGuiConfig_Canvas.h"

// engine
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UObject/SoftObjectPtr.h"

#include "KLDebugImGuiConfig.generated.h"

class UMaterialInterface;

UCLASS(config = Plugins, defaultconfig, DisplayName = "KLImGui")
class KLDEBUGIMGUIFRAMEWORK_API UKLDebugImGuiConfig final : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UKLDebugImGuiConfig();

    [[nodiscard]] static const UKLDebugImGuiConfig& Get();

    [[nodiscard]] const TSoftObjectPtr<UMaterialInterface>& GeOverlayMaterial() const;
    [[nodiscard]] const FKLDebugImGuiConfig_Canvas& GetCanvasConfig() const;

private:
    UPROPERTY(EditDefaultsOnly, config)
    FKLDebugImGuiConfig_Canvas CanvasConfig;

    UPROPERTY(EditDefaultsOnly, config)
    TSoftObjectPtr<UMaterialInterface> OverlayMaterial;
};

inline const UKLDebugImGuiConfig& UKLDebugImGuiConfig::Get()
{
    const UKLDebugImGuiConfig* Config = GetDefault<UKLDebugImGuiConfig>();
    checkf(Config != nullptr, TEXT("config should be valid"));
    return *Config;
}

inline const TSoftObjectPtr<UMaterialInterface>& UKLDebugImGuiConfig::GeOverlayMaterial() const
{
    return OverlayMaterial;
}

inline const FKLDebugImGuiConfig_Canvas& UKLDebugImGuiConfig::GetCanvasConfig() const
{
    return CanvasConfig;
}
