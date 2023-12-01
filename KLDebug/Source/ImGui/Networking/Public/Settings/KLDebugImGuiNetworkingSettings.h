// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Engine/DeveloperSettings.h"

#include "KLDebugImGuiNetworkingSettings.generated.h"

UCLASS(config = Plugins, defaultconfig, DisplayName = "KLDebugNetworking")
class KLDEBUGIMGUINETWORKING_API UKLDebugImGuiNetworkingSettings final : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UE_NODISCARD static const UKLDebugImGuiNetworkingSettings& Get();

    UE_NODISCARD int32 Client_GetPort() const;
    UE_NODISCARD uint32 Client_GetReadBufferSize() const;
    UE_NODISCARD uint32 Client_GetWriteBufferSize() const;
    UE_NODISCARD uint32 Client_GetMaxConnectionBacklog() const;
    UE_NODISCARD uint32 Client_GetConnectionTempDataSize() const;

    UE_NODISCARD uint32 Server_GetMaxClientsExptectedConnected() const;
    UE_NODISCARD uint32 Server_GetStartPortSearch() const;
    UE_NODISCARD int32 Server_GetMaxConnectionBacklog() const;
    UE_NODISCARD uint32 Server_GetMaxPortDiscoveryRetries() const;
    UE_NODISCARD uint32 Server_GetReadBufferSize() const;
    UE_NODISCARD uint32 Server_GetWriteBufferSize() const;
    UE_NODISCARD uint32 Server_GetConnectionTempDataSize() const;
    UE_NODISCARD uint32 Server_GetConnectionTempCompressDataSize() const;

private:
    // Client

    UPROPERTY(Config, Category = "ImGuiClient", EditDefaultsOnly)
    int32 ClientPort = 50000;

    UPROPERTY(Config, Category = "ImGuiClient", EditDefaultsOnly)
    uint32 ClientReadBufferSize = 2 * 1024 * 1024;

    UPROPERTY(Config, Category = "ImGuiClient", EditDefaultsOnly)
    uint32 ClientWriteBufferSize = 2 * 1024 * 1024;

    // The number of connections to queue before refusing them.
    UPROPERTY(Category = "ImGuiClient", config, EditDefaultsOnly)
    uint32 ClientMaxClientConnectionsBacklog = 3;

    UPROPERTY(Category = "ImGuiClient", config, EditDefaultsOnly)
    uint32 ClientTempDataSizePerConnection = 2 * 1024 * 1024;

    // Server

    UPROPERTY(Category = "ImGuiServer", config, EditDefaultsOnly)
    uint32 MaxClientsConnectedExpected = 10;

    // how many times we will try to discover a port for the client before give up
    UPROPERTY(Category = "ImGuiServer", config, EditDefaultsOnly)
    uint32 ServerStartPortSearch = 50001;

    // how many times we will try to discover a port for the client before give up
    UPROPERTY(Category = "ImGuiServer", config, EditDefaultsOnly)
    uint32 ServerMaxPortDiscoveryRetries = 10;

    // The number of connections to queue before refusing them.
    UPROPERTY(Category = "ImGuiServer", config, EditDefaultsOnly)
    int32 ServerMaxConnectionsBacklog = 10;

    UPROPERTY(Config, Category = "ImGuiServer", EditDefaultsOnly)
    uint32 ServerReadBufferSize = 2 * 1024 * 1024;

    UPROPERTY(Config, Category = "ImGuiServer", EditDefaultsOnly)
    uint32 ServerWriteBufferSize = 2 * 1024 * 1024;

    UPROPERTY(Config, Category = "ImGuiServer", EditDefaultsOnly)
    uint32 ServerTempDataSizePerConnection = 2 * 1024 * 1024;

    UPROPERTY(Config, Category = "ImGuiServer", EditDefaultsOnly)
    uint32 ServerTempCompressedDataSizePerConnection = 2 * 1024 * 1024;
};

inline const UKLDebugImGuiNetworkingSettings& UKLDebugImGuiNetworkingSettings::Get()
{
    const UKLDebugImGuiNetworkingSettings* Config = GetDefault<UKLDebugImGuiNetworkingSettings>();
    checkf(Config != nullptr, TEXT("config should be valid"));
    return *Config;
}

inline int32 UKLDebugImGuiNetworkingSettings::Client_GetPort() const
{
    return ClientPort;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Client_GetReadBufferSize() const
{
    return ClientReadBufferSize;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Client_GetWriteBufferSize() const
{
    return ClientWriteBufferSize;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Client_GetMaxConnectionBacklog() const
{
    return ClientMaxClientConnectionsBacklog;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Client_GetConnectionTempDataSize() const
{
    return ClientTempDataSizePerConnection;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Server_GetMaxClientsExptectedConnected() const
{
    return MaxClientsConnectedExpected;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Server_GetStartPortSearch() const
{
    return ServerStartPortSearch;
}

inline int32 UKLDebugImGuiNetworkingSettings::Server_GetMaxConnectionBacklog() const
{
    return ServerMaxPortDiscoveryRetries;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Server_GetMaxPortDiscoveryRetries() const
{
    return ServerMaxConnectionsBacklog;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Server_GetReadBufferSize() const
{
    return ServerReadBufferSize;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Server_GetWriteBufferSize() const
{
    return ServerWriteBufferSize;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Server_GetConnectionTempDataSize() const
{
    return ServerTempDataSizePerConnection;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Server_GetConnectionTempCompressDataSize() const
{
    return ServerTempCompressedDataSizePerConnection;
}