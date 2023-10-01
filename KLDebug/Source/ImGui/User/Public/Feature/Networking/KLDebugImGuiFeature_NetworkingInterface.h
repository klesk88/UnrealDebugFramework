#pragma once

// engine
#include "CoreMinimal.h"
#include "UObject/NameTypes.h"

class FKLDebugImGuiFeature_NetworkingGatherDataInput;
class FKLDebugImGuiFeature_NetworkingReceiveDataInput;

/*
* interface that can be implemented on features to allow them to send/receive messages from client server
 */
class KLDEBUGIMGUIUSER_API IKLDebugImGuiFeature_NetworkingInterface
{
public:
    virtual ~IKLDebugImGuiFeature_NetworkingInterface();

    UE_NODISCARD virtual bool ShouldGatherData(const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput) const = 0;
    virtual void GatherData(const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput) const = 0;
    virtual void ReceiveData(const FKLDebugImGuiFeature_NetworkingReceiveDataInput& _Input) = 0;

    UE_NODISCARD virtual uint8 GetVersion() const;
    UE_NODISCARD virtual bool Client_InformServerWhenActive() const;
};

inline bool IKLDebugImGuiFeature_NetworkingInterface::Client_InformServerWhenActive() const
{
    return false;
}

inline uint8 IKLDebugImGuiFeature_NetworkingInterface::GetVersion() const
{
    return 0;
}