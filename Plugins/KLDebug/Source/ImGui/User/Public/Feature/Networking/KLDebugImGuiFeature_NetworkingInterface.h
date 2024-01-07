// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "UObject/NameTypes.h"

class FArchive;
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

    // if true we will check the crc of the data buffer gather trough GatherData before sending the message
    // if is the same then we will skip the message
    UE_NODISCARD virtual bool ShouldVerifyCRCBeforeSendData() const;
};

inline bool IKLDebugImGuiFeature_NetworkingInterface::Client_InformServerWhenActive() const
{
    return false;
}

inline uint8 IKLDebugImGuiFeature_NetworkingInterface::GetVersion() const
{
    return 0;
}

inline bool IKLDebugImGuiFeature_NetworkingInterface::ShouldVerifyCRCBeforeSendData() const
{
    return true;
}
