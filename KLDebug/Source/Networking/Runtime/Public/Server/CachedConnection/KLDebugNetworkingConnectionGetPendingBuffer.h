// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugNetworkingPendingMessage;

class KLDEBUGNETWORKINGRUNTIME_API FKLDebugNetworkingConnectionGetPendingBuffer final : public FNoncopyable
{
public:
    explicit FKLDebugNetworkingConnectionGetPendingBuffer(TArray<FKLDebugNetworkingPendingMessage>& _PendingMessages, bool& _HasReadData);
    ~FKLDebugNetworkingConnectionGetPendingBuffer();

    UE_NODISCARD TArray<FKLDebugNetworkingPendingMessage>& GetPendingMessages() const;

private:
    TArray<FKLDebugNetworkingPendingMessage>& mPendingMessages;
    bool& mHasReadData;
};

inline TArray<FKLDebugNetworkingPendingMessage>& FKLDebugNetworkingConnectionGetPendingBuffer::GetPendingMessages() const
{
    return mPendingMessages;
}