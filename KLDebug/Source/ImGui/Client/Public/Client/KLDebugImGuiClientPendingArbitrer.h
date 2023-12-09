// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiClientManager.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"

class FSocket;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientPendingArbitrer final : public FNoncopyable
{
public:
    ~FKLDebugImGuiClientPendingArbitrer();

    void Init(FSocket& _Socket);
    void SendData();
    UE_NODISCARD bool HasSentData() const;
    UE_NODISCARD TArray<uint8>& GetDataToSendMutable();

private:
    void Shutdown();

private:
    TArray<uint8> mDataToSend;
    FSocket* mArbitrer = nullptr;
};

inline TArray<uint8>& FKLDebugImGuiClientPendingArbitrer::GetDataToSendMutable()
{
    return mDataToSend;
}

inline bool FKLDebugImGuiClientPendingArbitrer::HasSentData() const
{
    return mArbitrer == nullptr;
}
