#include "Config/KLDebugGameplayConfig.h"

UKLDebugGameplayConfig::UKLDebugGameplayConfig()
{
    CategoryName = TEXT("Plugins");
    SectionName = TEXT("KLImGuiGameplay");
}

void UKLDebugGameplayConfig::Load() const
{
    InputConfig.Load();
}
