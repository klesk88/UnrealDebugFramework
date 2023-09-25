#pragma once

// engine
#include "CoreMinimal.h"

/*
* interface that can be implemented on features to allow them to send/receive messages from client server
 */
class KLDEBUGIMGUINETWORKINGRUNTIME_API IKLDebugImGuiNetworking_FeatureInterface
{
public:
    virtual ~IKLDebugImGuiNetworking_FeatureInterface();

    UE_NODISCARD virtual bool Client_InformServerWhenActive() const;
};

inline bool IKLDebugImGuiNetworking_FeatureInterface::Client_InformServerWhenActive() const
{
    return false;
}
