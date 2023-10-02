#pragma once

// engine
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InstancedStruct.h"
#include "UObject/SoftObjectPtr.h"

#include "KLDebugNetworkingSettings.generated.h"

UCLASS(config=Plugins, defaultconfig, DisplayName = "KLDebugNetworking")
class KLDEBUGNETWORKINGRUNTIME_API UKLDebugNetworkingSettings final : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UE_NODISCARD static const UKLDebugNetworkingSettings& Get();

    UE_NODISCARD int32 GetPort() const;
    UE_NODISCARD uint32 GetReadBufferSize() const;
    UE_NODISCARD uint32 GetWriteBufferSize() const;
    UE_NODISCARD float GetClientReconnectionTime() const;

private:
    UPROPERTY(EditDefaultsOnly, config)
    int32 Port = 50000;

    UPROPERTY(EditDefaultsOnly, config)
    uint32 ReadBufferSize = 2 * 1024 * 1024;

    UPROPERTY(EditDefaultsOnly, config)
    uint32 WriteBufferSize = 2 * 1024 * 1024;

    //how often the client will try to reconnect to the server if the connection is lost (in seconds)
    UPROPERTY(EditDefaultsOnly, config, meta = (Units = "s"))
    float ClientReconnectionTime = 1.f;
};

inline const UKLDebugNetworkingSettings& UKLDebugNetworkingSettings::Get()
{
    const UKLDebugNetworkingSettings* Config = GetDefault<UKLDebugNetworkingSettings>();
    checkf(Config != nullptr, TEXT("config should be valid"));
    return *Config;
}

inline int32 UKLDebugNetworkingSettings::GetPort() const
{
    return Port;
}

inline uint32 UKLDebugNetworkingSettings::GetReadBufferSize() const
{
    return ReadBufferSize;
}

inline uint32 UKLDebugNetworkingSettings::GetWriteBufferSize() const
{
    return WriteBufferSize;
}

inline float UKLDebugNetworkingSettings::GetClientReconnectionTime() const
{
    return ClientReconnectionTime;
}
