#pragma once

#include "Tick/KLDebugImGuiTickObject.h"

// engine
#include "Containers/Array.h"
#include "Templates/UnrealTemplate.h"

class FSocket;
class FString;
class UWorld;

struct FKLDebugImGuiConfig_Networking;

class KLDEBUGIMGUI_API FKLDebugImGuiNetworkManager_Base : public FNoncopyable
{
public:
    virtual ~FKLDebugImGuiNetworkManager_Base() = default;

    virtual void Init(UWorld& _World) = 0;
    UE_NODISCARD virtual bool IsSocketRegistered() const = 0;

    void Clear();

protected:
    virtual void Tick(const float _DeltaTime) = 0;
    virtual void ClearChild();

    void InitTick(UWorld& _World);
    UE_NODISCARD const FKLDebugImGuiConfig_Networking& GetNetworkConfig() const;

    void UnregisterTick();
    UE_NODISCARD bool SendData(FSocket& _Socket, FBitWriter& _Writer) const;

private:
    FKLDebugImGuiTickObject mTickObject;
};

inline void FKLDebugImGuiNetworkManager_Base::ClearChild()
{
}
