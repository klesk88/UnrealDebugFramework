#pragma once

// engine
#include "CoreMinimal.h"
#include "UObject/NameTypes.h"

class FKLDebugImGuiNetworking_GatherDataInput;

/*
* interface that can be implemented on features to allow them to send/receive messages from client server
 */
class KLDEBUGIMGUINETWORKINGRUNTIME_API IKLDebugImGuiNetworking_FeatureInterface
{
public:
    virtual ~IKLDebugImGuiNetworking_FeatureInterface();

    UE_NODISCARD virtual bool ShouldGatherData(const FKLDebugImGuiNetworking_GatherDataInput& _GatherDataInput) const = 0;
    virtual void GatherData(const FKLDebugImGuiNetworking_GatherDataInput& _GatherDataInput) const = 0;
    
    UE_NODISCARD virtual uint8 GetVersion() const;
    UE_NODISCARD virtual bool Client_InformServerWhenActive() const;
};

inline bool IKLDebugImGuiNetworking_FeatureInterface::Client_InformServerWhenActive() const
{
    return false;
}

inline uint8 IKLDebugImGuiNetworking_FeatureInterface::GetVersion() const
{
    return 0;
}