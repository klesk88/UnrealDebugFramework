// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Templates/SubclassOf.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AKLDebugGameplayCameraController;
class ULocalPlayer;

class KLDEBUGGAMEPLAYRUNTIME_API FKLDebugGameplayCameraManager final : public FNoncopyable
{
public:
    explicit FKLDebugGameplayCameraManager();

    void ToogleDebugCamera(const TWeakObjectPtr<const ULocalPlayer>& _LocalPlayer);

    void UpdateCameraClass(const TSubclassOf<AKLDebugGameplayCameraController>& _NewCameraClass);

private:
    void EnableDebugCamera(const TWeakObjectPtr<const ULocalPlayer>& _LocalPlayer);
    void DisableDebugCamera(const TWeakObjectPtr<const ULocalPlayer>& _LocalPlayer);

private:
    TSubclassOf<AKLDebugGameplayCameraController> mCameraClass;
    bool mDebugCameraIsEnable = false;
};