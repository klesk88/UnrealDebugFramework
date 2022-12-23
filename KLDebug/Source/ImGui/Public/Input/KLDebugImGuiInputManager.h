#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGIMGUI_API FKLDebugImGuiInputManager final : public FNoncopyable
{
public:
    void Init();
    void Shutdown();

    void ToogleImGuiSystem();

private:
    bool mIsImGuiEnable = false;
};
