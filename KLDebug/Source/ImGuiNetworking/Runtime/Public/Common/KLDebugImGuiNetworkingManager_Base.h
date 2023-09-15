#pragma once

//utils moduule
#include "Utils/Public/Tick/KLDebugUtilsTickObject.h"

// engine
#include "Containers/Array.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FSocket;
class FString;
class UKLDebugImGuiNetworkingSettings;
class UWorld;

class KLDEBUGIMGUINETWORKINGRUNTIME_API FKLDebugImGuiNetworkingManager_Base : public FNoncopyable
{
public:
    virtual ~FKLDebugImGuiNetworkingManager_Base() = default;

    void Init(UWorld& _World);
    UE_NODISCARD virtual bool IsSocketRegistered() const = 0;

    void Clear(const UWorld& _World);

protected:
    virtual void InitChild(UWorld& _World);
    virtual void ClearChild();
    virtual void Tick(const float _DeltaTime) = 0;

    void InitTick(UWorld& _World);
    UE_NODISCARD const UKLDebugImGuiNetworkingSettings& GetNetworkConfig() const;

    void UnregisterTick();
    UE_NODISCARD bool SendData(FSocket& _Socket, FBitWriter& _Writer) const;

    UE_NODISCARD UWorld& GetWorldMutable() const;
    UE_NODISCARD const UWorld& GetWorld() const;

private:
    FKLDebugUtilsTickObject mTickObject;
    TWeakObjectPtr<UWorld> mWorld;
};

inline void FKLDebugImGuiNetworkingManager_Base::InitChild(UWorld& _World)
{
}

inline void FKLDebugImGuiNetworkingManager_Base::ClearChild()
{
}

inline const UWorld& FKLDebugImGuiNetworkingManager_Base::GetWorld() const
{
    return GetWorldMutable();
}