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

    UE_NODISCARD bool IsEnable() const;

private:
    bool mIsImGuiEnable = false;
};

inline bool FKLDebugImGuiInputManager::IsEnable() const
{
    return mIsImGuiEnable;
}