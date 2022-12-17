#pragma once

//debug utils module
#include "Utils/Public/Picker/KLDebugUtilsPicker.h"

// engine
#include "CoreMinimal.h"
#include "Delegates/IDelegateInstance.h"
#include "Subsystems/WorldSubsystem.h"

#include "KLDebugImGuiWorldSubsystem.generated.h"

class IKLDebugImGuiFeatureInterface;

UCLASS(Transient)
class KLDEBUGIMGUI_API UKLDebugImGuiWorldSubsystem final : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    bool ShouldCreateSubsystem(UObject* _Outer) const final;
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;

private:
    void RegisterCallbacks();
    void UnregisterCallbacks();

    void OnObjectSelected(UObject* _Object);

private:
    FKLDebugUtilsPicker mPicker;
};
