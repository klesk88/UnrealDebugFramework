// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"

class FArchive;
class FKLDebugNetworkingMessageSerializeInput;

class KLDEBUGNETWORKINGRUNTIME_API IKLDebugNetworkingMessageInterface
{
public:
    virtual ~IKLDebugNetworkingMessageInterface() = default;

    UE_NODISCARD virtual uint16 GetMessageType() const = 0;
    UE_NODISCARD virtual uint16 GetMessageEnumType() const = 0;
    UE_NODISCARD virtual bool IsValid() const;

    UE_NODISCARD virtual bool SupportsEmptyData() const;
    UE_NODISCARD virtual uint8 GetMessageVersion() const;

    void Serialize(FArchive& _Archive);

protected:
    virtual void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) = 0;
};

inline uint8 IKLDebugNetworkingMessageInterface::GetMessageVersion() const
{
    return 0;
}

inline bool IKLDebugNetworkingMessageInterface::IsValid() const
{
    return true;
}

inline bool IKLDebugNetworkingMessageInterface::SupportsEmptyData() const
{
    return false;
}