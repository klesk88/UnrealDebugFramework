// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "ServerPortGetter/KLDebugImGuiClientServerPortGetterBase.h"

// modules
#include "ImGui/Networking/Public/Helpers/KLDebugImGuiNetworkingHelpers.h"

// engine
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/NetConnection.h"
#include "Engine/NetDriver.h"
#include "Engine/World.h"
#include "Math/NumericLimits.h"

bool IKLDebugImGuiClientServerPortGetterBase::GatherWorldData(const UWorld& _World, uint32& _ServerIP, int32& _ServerPort, FUniqueNetIdRepl& _LocalPlayerNetID) const
{
    const UNetDriver* WorldNetDriver = _World.GetNetDriver();
    const UNetConnection* NetConnection = WorldNetDriver ? WorldNetDriver->ServerConnection : nullptr;
    if (!NetConnection || !NetConnection->RemoteAddr.IsValid())
    {
        ensureMsgf(false, TEXT("we expect the world to be fully initialize here"));
        return false;
    }

    const UGameInstance* GameInstance = _World.GetGameInstance();
    _LocalPlayerNetID = GameInstance ? GameInstance->GetPrimaryPlayerUniqueIdRepl() : FUniqueNetIdRepl();
    if (!_LocalPlayerNetID.IsValid())
    {
        return false;
    }

    _ServerIP = TNumericLimits<uint32>::Max();
    NetConnection->RemoteAddr->GetIp(_ServerIP);
    _ServerPort = NetConnection->URL.Port;
    return _ServerIP != TNumericLimits<uint32>::Max();
}

bool IKLDebugImGuiClientServerPortGetterBase::IsWorldDataReady(const UWorld& _World) const
{
    uint32 ServerIp;
    int32 ClientIp;
    FUniqueNetIdRepl LocalPlayerNetID;
    return GatherWorldData(_World, ServerIp, ClientIp, LocalPlayerNetID);
}
