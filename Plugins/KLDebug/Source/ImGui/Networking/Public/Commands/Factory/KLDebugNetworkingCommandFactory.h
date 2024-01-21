// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Internal/StaticMemory/KLDebugUtilsStaticMemoryFactory.h"

class IKLDebugUserNetworkingCommandInterface;

class KLDEBUGIMGUINETWORKING_API FKLDebugNetworkingCommandFactory final : public TKLDebugUtilsStaticMemoryFactory<IKLDebugUserNetworkingCommandInterface>
{
public:
    void Init();

    UE_NODISCARD const IKLDebugUserNetworkingCommandInterface* TryGetCommand(const FName& _CommandID) const;
};

inline const IKLDebugUserNetworkingCommandInterface* FKLDebugNetworkingCommandFactory::TryGetCommand(const FName& _CommandID) const
{
    return TryGetData(_CommandID);
}