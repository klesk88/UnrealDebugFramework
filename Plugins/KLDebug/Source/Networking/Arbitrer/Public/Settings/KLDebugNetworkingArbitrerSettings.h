// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Engine/DeveloperSettings.h"

#include "KLDebugNetworkingArbitrerSettings.generated.h"

UCLASS(config = Plugins, defaultconfig, DisplayName = "KLDebugNetworking")
class KLDEBUGNETWORKINGARBITRER_API UKLDebugNetworkingArbitrerSettings final : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    [[nodiscard]] static const UKLDebugNetworkingArbitrerSettings& Get();

    [[nodiscard]] uint32 GetPort() const;
    [[nodiscard]] uint32 GetStartClientPortRange() const;
    [[nodiscard]] uint32 GetEndClientPortRange() const;
    [[nodiscard]] uint32 GetServerPortListeningStart() const;
    [[nodiscard]] uint32 GetServerPortListeningEnd() const;
    [[nodiscard]] uint32 GetReceiveBufferSize() const;
    [[nodiscard]] uint32 GetWriteBufferSize() const;
    [[nodiscard]] float GetMaxTimeForClientAnswer() const;

private:
    // this is hte port use by the arbitrer service
    UPROPERTY(Config, Category = "Arbitrer", EditDefaultsOnly)
    uint32 ServerPort = 50000;

    // this is the port use by the client machine to listen for answers from the arbitrer service
    UPROPERTY(Config, Category = "Arbitrer", EditDefaultsOnly)
    uint32 ClientStartPortRange = 50010;

    // this is the port use by the client machine to listen for answers from the arbitrer service
    UPROPERTY(Config, Category = "Arbitrer", EditDefaultsOnly)
    uint32 ClientEndPortRange = 50090;

    // the port that we use on the server to listen to arbitrer communications
    UPROPERTY(Config, Category = "Arbitrer", EditDefaultsOnly)
    uint32 ServerPortListeningStartRange = 50100;

    UPROPERTY(Config, Category = "Arbitrer", EditDefaultsOnly)
    uint32 ServerPortListeningEndRange = 50200;

    UPROPERTY(Config, Category = "Arbitrer", EditDefaultsOnly)
    uint32 ReceiveBufferSize = 2 * 1024 * 1024;

    UPROPERTY(Config, Category = "Arbitrer", EditDefaultsOnly)
    uint32 WriteBufferSize = 2 * 1024 * 1024;

    UPROPERTY(Config, Category = "Arbitrer", EditDefaultsOnly)
    float MaxTimeForClientAnswer = 5.f;
};

inline const UKLDebugNetworkingArbitrerSettings& UKLDebugNetworkingArbitrerSettings::Get()
{
    const UKLDebugNetworkingArbitrerSettings* Config = GetDefault<UKLDebugNetworkingArbitrerSettings>();
    checkf(Config != nullptr, TEXT("config should be valid"));
    return *Config;
}

inline uint32 UKLDebugNetworkingArbitrerSettings::GetPort() const
{
    return ServerPort;
}

inline uint32 UKLDebugNetworkingArbitrerSettings::GetStartClientPortRange() const
{
    return ClientStartPortRange;
}

inline uint32 UKLDebugNetworkingArbitrerSettings::GetEndClientPortRange() const
{
    return ClientEndPortRange;
}

inline uint32 UKLDebugNetworkingArbitrerSettings::GetServerPortListeningStart() const
{
    return ServerPortListeningStartRange;
}

inline uint32 UKLDebugNetworkingArbitrerSettings::GetServerPortListeningEnd() const
{
    return ServerPortListeningEndRange;
}

inline uint32 UKLDebugNetworkingArbitrerSettings::GetReceiveBufferSize() const
{
    return ReceiveBufferSize;
}

inline uint32 UKLDebugNetworkingArbitrerSettings::GetWriteBufferSize() const
{
    return WriteBufferSize;
}

inline float UKLDebugNetworkingArbitrerSettings::GetMaxTimeForClientAnswer() const
{
    return MaxTimeForClientAnswer;
}