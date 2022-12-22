#pragma once

// engine
#include "CoreMinimal.h"

enum class EFeatureEntryType
{
    Default = 0,
    Subsystem_Engine,
    Subsystem_Editor,
    Subsystem_GameInstance,
    Subsystem_World,
    Subsystem_Player,

    Count
};
