// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Engine/DeveloperSettings.h"

#include "KLDebugImGuiNetworkingSettings.generated.h"

#if WITH_EDITOR
struct FPropertyChangedEvent;
#endif

UCLASS(config = Plugins, defaultconfig, DisplayName = "KLDebugNetworking")
class KLDEBUGIMGUINETWORKING_API UKLDebugImGuiNetworkingSettings final : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    [[nodiscard]] static const UKLDebugImGuiNetworkingSettings& Get();

    [[nodiscard]] uint32 Client_GetReadBufferSize() const;
    [[nodiscard]] uint32 Client_GetWriteBufferSize() const;
    [[nodiscard]] uint32 Client_GetConnectionTempDataSize() const;

    [[nodiscard]] uint32 Server_GetMaxClientsExptectedConnected() const;
    [[nodiscard]] uint32 Server_GetStartPortRange() const;
    [[nodiscard]] uint32 Server_GetEndPortRange() const;
    [[nodiscard]] uint32 Server_GetMaxConnectionBacklog() const;
    [[nodiscard]] uint32 Server_GetReadBufferSize() const;
    [[nodiscard]] uint32 Server_GetWriteBufferSize() const;
    [[nodiscard]] uint32 Server_GetConnectionTempDataSize() const;
    [[nodiscard]] uint32 Server_GetConnectionTempCompressDataSize() const;

    [[nodiscard]] uint32 Common_GetMaxMessageDataSizeBeforeSplit() const;
    [[nodiscard]] uint32 Common_GetMaxMessageDataSizeBeforeCompression() const;
    [[nodiscard]] uint32 Common_GetCompressMinBytesSaved() const;
    [[nodiscard]] uint32 Common_GetCompressMinPercentSaved() const;

private:
    // Client

    UPROPERTY(Config, Category = "ImGuiClient", EditDefaultsOnly)
    uint32 ClientReadBufferSize = 2 * 1024 * 1024;

    UPROPERTY(Config, Category = "ImGuiClient", EditDefaultsOnly)
    uint32 ClientWriteBufferSize = 2 * 1024 * 1024;

    UPROPERTY(Category = "ImGuiClient", config, EditDefaultsOnly)
    uint32 ClientTempDataSizePerConnection = 2 * 1024 * 1024;

    // Server

    UPROPERTY(Category = "ImGuiServer", config, EditDefaultsOnly)
    uint32 MaxClientsConnectedExpected = 10;

    UPROPERTY(Category = "ImGuiServer", config, EditDefaultsOnly)
    uint32 ServerStartPortRange = 50300;

    UPROPERTY(Category = "ImGuiServer", config, EditDefaultsOnly)
    uint32 ServerEndPortRange = 50400;

    // The number of connections to queue before refusing them.
    UPROPERTY(Category = "ImGuiServer", config, EditDefaultsOnly)
    uint32 ServerMaxConnectionsBacklog = 10;

    UPROPERTY(Config, Category = "ImGuiServer", EditDefaultsOnly)
    uint32 ServerReadBufferSize = 2 * 1024 * 1024;

    UPROPERTY(Config, Category = "ImGuiServer", EditDefaultsOnly)
    uint32 ServerWriteBufferSize = 2 * 1024 * 1024;

    UPROPERTY(Config, Category = "ImGuiServer", EditDefaultsOnly)
    uint32 ServerTempDataSizePerConnection = 2 * 1024 * 1024;

    UPROPERTY(Config, Category = "ImGuiServer", EditDefaultsOnly)
    uint32 ServerTempCompressedDataSizePerConnection = 2 * 1024 * 1024;

    // determine the max size of the data to send in a single message. If the message is higher then this size
    // it will be splitted in multiple ones.
    UPROPERTY(Config, Category = "Common", EditDefaultsOnly, meta = (Units = "Bytes"))
    uint32 CommonMaxMessageDataSizeBeforeSplit = 1024;

    UPROPERTY(Config, Category = "Common", EditDefaultsOnly, meta = (Units = "Bytes"))
    uint32 CommonMaxMessageDataSizeBeforeCompression = 1024;

    UPROPERTY(Config, Category = "Common", EditDefaultsOnly, meta = (Units = "Bytes"))
    uint32 CompressMinBytesSaved = 10;

    UPROPERTY(Config, Category = "Common", EditDefaultsOnly, meta = (Units = "%", ClampMin = 0, ClampMax = 100))
    uint32 CompressMinPercentSaved = 5;

#if WITH_EDITOR
private:
    // UDeveloperSettings
    void PostEditChangeProperty(FPropertyChangedEvent& _PropertyChangedEvent) final;
    void PostLoad() final;
    // UDeveloperSettings

    void EditorCheckPorts() const;
#endif
};

inline const UKLDebugImGuiNetworkingSettings& UKLDebugImGuiNetworkingSettings::Get()
{
    const UKLDebugImGuiNetworkingSettings* Config = GetDefault<UKLDebugImGuiNetworkingSettings>();
    checkf(Config != nullptr, TEXT("config should be valid"));
    return *Config;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Client_GetReadBufferSize() const
{
    return ClientReadBufferSize;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Client_GetWriteBufferSize() const
{
    return ClientWriteBufferSize;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Client_GetConnectionTempDataSize() const
{
    return ClientTempDataSizePerConnection;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Server_GetMaxClientsExptectedConnected() const
{
    return MaxClientsConnectedExpected;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Server_GetStartPortRange() const
{
    return ServerStartPortRange;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Server_GetEndPortRange() const
{
    return ServerEndPortRange;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Server_GetMaxConnectionBacklog() const
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

inline uint32 UKLDebugImGuiNetworkingSettings::Common_GetMaxMessageDataSizeBeforeSplit() const
{
    return CommonMaxMessageDataSizeBeforeSplit;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Common_GetMaxMessageDataSizeBeforeCompression() const
{
    return CommonMaxMessageDataSizeBeforeCompression;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Common_GetCompressMinBytesSaved() const
{
    return CompressMinBytesSaved;
}

inline uint32 UKLDebugImGuiNetworkingSettings::Common_GetCompressMinPercentSaved() const
{
    return CompressMinPercentSaved;
}
