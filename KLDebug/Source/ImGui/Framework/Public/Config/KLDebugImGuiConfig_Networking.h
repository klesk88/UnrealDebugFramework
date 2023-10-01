#pragma once

// engine
#include "Containers/UnrealString.h"
#include "UObject/Class.h"

#include "KLDebugImGuiConfig_Networking.generated.h"

USTRUCT()
struct KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiConfig_Networking
{
    GENERATED_BODY()

public:
    UE_NODISCARD int32 GetPort() const;
    UE_NODISCARD uint32 GetReadBufferSize() const;
    UE_NODISCARD uint32 GetWriteBufferSize() const;

private:
    UPROPERTY(EditDefaultsOnly, config)
    int32 Port = 50000;

    UPROPERTY(EditDefaultsOnly, config)
    uint32 ReadBufferSize = 2 * 1024 * 1024;

    UPROPERTY(EditDefaultsOnly, config)
    uint32 WriteBufferSize = 2 * 1024 * 1024;
};

inline int32 FKLDebugImGuiConfig_Networking::GetPort() const
{
    return Port;
}

inline uint32 FKLDebugImGuiConfig_Networking::GetReadBufferSize() const
{
    return ReadBufferSize;
}

inline uint32 FKLDebugImGuiConfig_Networking::GetWriteBufferSize() const
{
    return WriteBufferSize;
}
