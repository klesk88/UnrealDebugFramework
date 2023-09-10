#pragma once

// engine
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InstancedStruct.h"
#include "UObject/SoftObjectPtr.h"

#include "KLDebugImGuiConfig.generated.h"

class UMaterialInterface;

UCLASS(config=Plugins, defaultconfig, DisplayName = "KLImGui")
class KLDEBUGIMGUI_API UKLDebugImGuiConfig final : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UKLDebugImGuiConfig();

    UE_NODISCARD static const UKLDebugImGuiConfig& Get();

    UE_NODISCARD const FInstancedStruct& GetImGuiWindow() const;
    UE_NODISCARD const TSoftObjectPtr<UMaterialInterface>& GeOverlayMaterial() const;

private:
    UPROPERTY(EditDefaultsOnly, config, NoClear, meta = (BaseStruct = "/Script/KLDebugImGui.KLDebugImGuiWindow"))
    FInstancedStruct ImGuiWindowManager;

    UPROPERTY(EditDefaultsOnly, config)
    TSoftObjectPtr<UMaterialInterface> OverlayMaterial;
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

inline const TSoftObjectPtr<UMaterialInterface>& UKLDebugImGuiConfig::GeOverlayMaterial() const
{
    return OverlayMaterial;
}