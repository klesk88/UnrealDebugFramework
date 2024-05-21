// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "GenericPlatform/GenericPlatform.h"

class FKLDebugArbitrerDebugServerInfo
{
public:
    explicit FKLDebugArbitrerDebugServerInfo(const uint32 _DebugPort, const uint8 _HeaderVersion);

    [[nodiscard]] uint32 GetDebugPort() const;
    [[nodiscard]] uint8 GetHeaderVersion() const;

private:
    uint32 mDebugPort = 0;
    uint8 mHeaderVersion = 0;
};

inline uint32 FKLDebugArbitrerDebugServerInfo::GetDebugPort() const
{
    return mDebugPort;
}

inline uint8 FKLDebugArbitrerDebugServerInfo::GetHeaderVersion() const
{
    return mHeaderVersion;
}