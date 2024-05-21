// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"

class FArchive;

class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingMessageSerializeInput final : public FNoncopyable
{
public:
    explicit FKLDebugNetworkingMessageSerializeInput(const uint8 _MessageVersion, FArchive& _Archive);

    [[nodiscard]] uint8 GetMessageVersion() const;
    [[nodiscard]] FArchive& GetArchive() const;

protected:
    FArchive& mArchive;
    uint8 mMessageVersion = 0;
};

inline uint8 FKLDebugNetworkingMessageSerializeInput::GetMessageVersion() const
{
    return mMessageVersion;
}

inline FArchive& FKLDebugNetworkingMessageSerializeInput::GetArchive() const
{
    return mArchive;
}