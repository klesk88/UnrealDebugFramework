// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Settings/KLDebugImGuiNetworkingSettings.h"

#if WITH_EDITOR
// modules
#include "Networking/Arbitrer/Public/Settings/KLDebugNetworkingArbitrerSettings.h"
#include "Utils/Public/KLDebugLog.h"

// engine
#include "UObject/UnrealType.h"
#endif

#if WITH_EDITOR

void UKLDebugImGuiNetworkingSettings::PostEditChangeProperty(FPropertyChangedEvent& _PropertyChangedEvent)
{
    Super::PostEditChangeProperty(_PropertyChangedEvent);

    if (_PropertyChangedEvent.Property != nullptr && (_PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UKLDebugImGuiNetworkingSettings, ServerStartPortRange) || _PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UKLDebugImGuiNetworkingSettings, ServerEndPortRange)))
    {
        EditorCheckPorts();
    }
}

void UKLDebugImGuiNetworkingSettings::PostLoad()
{
    Super::PostLoad();

    EditorCheckPorts();
}

void UKLDebugImGuiNetworkingSettings::EditorCheckPorts() const
{
    const UKLDebugNetworkingArbitrerSettings& Settings = UKLDebugNetworkingArbitrerSettings::Get();
    const uint32 ArbitrerPort = Settings.GetPort();
    if (ServerStartPortRange <= Settings.GetStartClientPortRange() && ServerStartPortRange <= Settings.GetEndClientPortRange())
    {
        UE_LOG(LogKL_Debug, Error, TEXT("ServerStartPortRange contains the port dedicated for the client arbitrer"));
    }

    if (ServerEndPortRange <= Settings.GetStartClientPortRange() && ServerEndPortRange <= Settings.GetEndClientPortRange())
    {
        UE_LOG(LogKL_Debug, Error, TEXT("ServerEndPortRange contains the port dedicated for the client arbitrer"));
    }

    if (ServerStartPortRange <= ArbitrerPort && ArbitrerPort <= ServerEndPortRange)
    {
        UE_LOG(LogKL_Debug, Error, TEXT("server port range contains the port dedicated for the arbitrer. Arbitrer port is set at [d]"), ArbitrerPort);
    }
}

#endif