#include "Message/KLDebugImGuiNetworkingMessageBase.h"

//engine
#include "Engine/NetConnection.h"
#include "Engine/NetDriver.h"
#include "Engine/PackageMapClient.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Templates/SharedPointer.h"

void FKLDebugImGuiNetworkingMessageBase::Write(const UWorld& _World, FBitWriter& _BitWriter)
{
    EKLDebugNetworkMessageTypes MessageType = GetMessageType();
    _BitWriter << MessageType;

    WriteChild(_World, _BitWriter);
}

void FKLDebugImGuiNetworkingMessageBase::Read(const UWorld& _World, FBitReader& _BitReader)
{
    ReadChild(_World, _BitReader);
}

FNetworkGUID FKLDebugImGuiNetworkingMessageBase::TryGetNetworkGuid(const UObject& _Object) const
{
    const UNetDriver* NetDriver = nullptr;
    if (const UWorld* World = Cast<const UWorld>(&_Object))
    {
        NetDriver = World->GetNetDriver();
    }
    else if(const AActor* Actor = Cast<const AActor>(&_Object))
    {
        NetDriver = Actor->GetNetDriver();
    }
    else
    {
        ensureMsgf(false, TEXT("could not find net driver"));
        return FNetworkGUID();
    }

    const TSharedPtr<FNetGUIDCache> GuidCache = NetDriver ? NetDriver->GuidCache : nullptr;
    if (!GuidCache.IsValid())
    {
        return FNetworkGUID();
    }

    //based on FReplayHelper::SetExternalDataForObject
    const FNetworkGUID NetworkGUID = GuidCache->NetGUIDLookup.FindRef(&_Object);
    return NetworkGUID;
}

const UObject* FKLDebugImGuiNetworkingMessageBase::TryGetObjectFromNetworkGUID(const UWorld& _World, const FNetworkGUID& _NetworkGUID) const
{
    const UNetDriver* NetDriver = _World.GetNetDriver();
    const TSharedPtr<FNetGUIDCache> GuidCache = NetDriver ? NetDriver->GuidCache : nullptr;
    if (!GuidCache)
    {
        return nullptr;
    }

    const UObject* Object = GuidCache->GetObjectFromNetGUID(_NetworkGUID, true);
    return Object;
}
