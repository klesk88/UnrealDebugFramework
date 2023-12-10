// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiInputManager final : public FNoncopyable
{
public:
    void Init();
    void Shutdown();

    void ToogleImGuiSystem();
    void ToogleImGuiInput();

    UE_NODISCARD bool IsEnable() const;

private:
    bool mIsImGuiEnable = false;
};

inline bool FKLDebugImGuiInputManager::IsEnable() const
{
    return mIsImGuiEnable;
}