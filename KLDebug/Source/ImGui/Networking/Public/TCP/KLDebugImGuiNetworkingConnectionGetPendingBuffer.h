#pragma once

//engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiNetworkingPendingMessage;

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingConnectionGetPendingBuffer final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiNetworkingConnectionGetPendingBuffer(TArray<FKLDebugImGuiNetworkingPendingMessage>& _PendingMessages, bool& _HasReadData);
    ~FKLDebugImGuiNetworkingConnectionGetPendingBuffer();

    UE_NODISCARD TArray<FKLDebugImGuiNetworkingPendingMessage>& GetPendingMessages() const;

private:
    TArray<FKLDebugImGuiNetworkingPendingMessage>& mPendingMessages;
    bool& mHasReadData;
};

inline TArray<FKLDebugImGuiNetworkingPendingMessage>& FKLDebugImGuiNetworkingConnectionGetPendingBuffer::GetPendingMessages() const
{
    return mPendingMessages;
}