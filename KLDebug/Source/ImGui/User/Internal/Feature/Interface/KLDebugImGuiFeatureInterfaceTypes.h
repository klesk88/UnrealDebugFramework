#pragma once

//engine
#include "CoreMinimal.h"
#include "HAL/Platform.h"

enum class KLDEBUGIMGUIUSER_API EImGuiInterfaceType : uint8
{
    SELECTABLE = 0,
    UNIQUE,

    COUNT
};