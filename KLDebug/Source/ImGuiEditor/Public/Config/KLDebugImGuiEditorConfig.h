#pragma once

#include "Input/KLDebugImGuiEditorInputConfig.h"

// engine
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "KLDebugImGuiEditorConfig.generated.h"

UCLASS(config=Plugins, defaultconfig, DisplayName = "KLImGuiEditor")
class KLDEBUGIMGUIEDITOR_API UKLDebugImGuiEditorConfig final : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UKLDebugImGuiEditorConfig();

    UE_NODISCARD static const UKLDebugImGuiEditorConfig& Get();
    UE_NODISCARD const FKLDebugImGuiEditorInputConfig& GetInputConfig() const;

private:
    void OnEnableImGui();

private:
    UPROPERTY(EditDefaultsOnly, config, Category = Input)
    FKLDebugImGuiEditorInputConfig InputConfig;
};

inline const UKLDebugImGuiEditorConfig& UKLDebugImGuiEditorConfig::Get()
{
    const UKLDebugImGuiEditorConfig* Config = GetDefault<UKLDebugImGuiEditorConfig>();
    checkf(Config != nullptr, TEXT("config should be valid"));
    return *Config;
}

inline const FKLDebugImGuiEditorInputConfig& UKLDebugImGuiEditorConfig::GetInputConfig() const
{
    return InputConfig;
}
