// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Helpers/KLDebugNetworkingHelpers.h"

// engine
#include "Engine/NetConnection.h"
#include "Engine/NetDriver.h"
#include "Engine/PackageMapClient.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Templates/SharedPointer.h"

namespace KL::Debug::Networking::Helpers
{
    FNetworkGUID TryGetNetworkGuid(const UObject& _Object)
    {
        // look also in case at NetworkPredictionDebug::FindObjectNetGUID

        const UNetDriver* NetDriver = nullptr;
        if (const UWorld* World = Cast<const UWorld>(&_Object))
        {
            NetDriver = World->GetNetDriver();
        }
        else if (const AActor* Actor = Cast<const AActor>(&_Object))
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

        // based on FReplayHelper::SetExternalDataForObject
        const FNetworkGUID NetworkGUID = GuidCache->NetGUIDLookup.FindRef(&_Object);
        return NetworkGUID;
    }

    UObject* TryGetObjectFromNetworkGUIDMutable(const UWorld& _World, const FNetworkGUID& _NetworkGUID)
    {
        const UNetDriver* NetDriver = _World.GetNetDriver();
        const TSharedPtr<FNetGUIDCache> GuidCache = NetDriver ? NetDriver->GuidCache : nullptr;
        if (!GuidCache)
        {
            return nullptr;
        }

        UObject* Object = GuidCache->GetObjectFromNetGUID(_NetworkGUID, true);
        return Object;
    }

    const UObject* TryGetObjectFromNetworkGUID(const UWorld& _World, const FNetworkGUID& _NetworkGUID)
    {
        return TryGetObjectFromNetworkGUIDMutable(_World, _NetworkGUID);
    }
}    // namespace KL::Debug::Networking::Helpers
