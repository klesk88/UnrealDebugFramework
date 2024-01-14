// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

// modules
#include "Utils/Public/StaticMemory/KLDebugUtilsStaticMemoryFactory.h"

class IKLDebugUserNetworkingCommandInterface;

class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingCommandFactory final : public TKLDebugUtilsStaticMemoryFactory<IKLDebugUserNetworkingCommandInterface>
{
public:
    void Init();

    UE_NODISCARD const IKLDebugUserNetworkingCommandInterface* TryGetCommand(const FName& _CommandID) const;
};

inline const IKLDebugUserNetworkingCommandInterface* FKLDebugNetworkingCommandFactory::TryGetCommand(const FName& _CommandID) const
{
    return TryGetData(_CommandID);
}