// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

// engine
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class IKLDebugUserNetworkingCommandInterface;

class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingCommandFactory final : public FNoncopyable
{
public:
    void Init();

    UE_NODISCARD const IKLDebugUserNetworkingCommandInterface* TryGetCommand(const FName& _CommandID) const;

private:
    TMap<FName, uint32> mIDToCommand;
    TArray<uint8> mCommandsPool;
};
