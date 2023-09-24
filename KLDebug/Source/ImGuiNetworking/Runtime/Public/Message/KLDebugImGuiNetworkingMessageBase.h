#pragma once

#include "Message/KLDebugImGuiNetworkingMessageTypes.h"

//engine
#include "CoreMinimal.h"
#include "Misc/NetworkGuid.h"
#include "Serialization/BitReader.h"
#include "Serialization/BitWriter.h"

class UObject;
class UWorld;

class KLDEBUGIMGUINETWORKINGRUNTIME_API FKLDebugImGuiNetworkingMessageBase
{
public:
    virtual ~FKLDebugImGuiNetworkingMessageBase() = default;

    void Write(const UWorld& _World, FBitWriter& _BitWriter);
    void Read(const UWorld& _World, FBitReader& _BitReader);

protected:
    UE_NODISCARD virtual EKLDebugNetworkMessageTypes GetMessageType() const = 0;
    UE_NODISCARD virtual bool CanWrite(const UWorld& _World) const = 0;
    virtual void WriteChild(const UWorld& _World, FBitWriter& _BitWriter) = 0;
    virtual void ReadChild(const UWorld& _World, FBitReader& _BitReader) = 0;

    UE_NODISCARD FNetworkGUID TryGetNetworkGuid(const UObject& _Object) const;
    UE_NODISCARD const UObject* TryGetObjectFromNetworkGUID(const UWorld& _World, const FNetworkGUID& _NetworkGUID) const;
};
