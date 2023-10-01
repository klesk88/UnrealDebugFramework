#pragma once

#include "Message/KLDebugImGuiNetworkingMessageTypes.h"

//modules
#include "Networking/Runtime/Public/Helpers/KLDebugImGuiNetworkingHelpers.h"

//engine
#include "CoreMinimal.h"
#include "Misc/NetworkGuid.h"
#include "Serialization/Archive.h"

class UObject;
class UWorld;

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingMessageBase
{
public:
    virtual ~FKLDebugImGuiNetworkingMessageBase() = default;

    UE_NODISCARD virtual bool CanWrite(const UWorld& _World) const = 0;

    void Write(const UWorld& _World, FArchive& _BitWriter);
    void Read(const UWorld& _World, FArchive& _BitReader);

protected:
    UE_NODISCARD virtual EKLDebugNetworkMessageTypes GetMessageType() const = 0;
    virtual void WriteChild(const UWorld& _World, FArchive& _BitWriter) = 0;
    virtual void ReadChild(const UWorld& _World, FArchive& _BitReader) = 0;

    UE_NODISCARD FNetworkGUID TryGetNetworkGuid(const UObject& _Object) const;
    UE_NODISCARD const UObject* TryGetObjectFromNetworkGUID(const UWorld& _World, const FNetworkGUID& _NetworkGUID) const;
};

inline FNetworkGUID FKLDebugImGuiNetworkingMessageBase::TryGetNetworkGuid(const UObject& _Object) const
{
    return KL::Debug::Networking::Helpers::TryGetNetworkGuid(_Object);
}

inline const UObject* FKLDebugImGuiNetworkingMessageBase::TryGetObjectFromNetworkGUID(const UWorld& _World, const FNetworkGUID& _NetworkGUID) const
{
    return KL::Debug::Networking::Helpers::TryGetObjectFromNetworkGUID(_World, _NetworkGUID);
}

