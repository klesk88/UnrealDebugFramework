#pragma once

// engine
#include "CoreMinimal.h"

/*
 */
class KLDEBUGIMGUINETWORKINGRUNTIME_API IKLDebugImGuiNetworing_FeatureInterface
{
public:
    virtual ~IKLDebugImGuiNetworing_FeatureInterface();

    UE_NODISCARD virtual bool Client_InformServerWhenActive() const;
};

inline bool IKLDebugImGuiNetworing_FeatureInterface::Client_InformServerWhenActive() const
{
    return false;
}
