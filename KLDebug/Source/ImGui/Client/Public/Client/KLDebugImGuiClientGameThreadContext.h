// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Subsystem/Engine/KLDebugImGuiClientData.h"

// modules
#include "ImGui/Networking/Public/Subsystem/Engine/KLDebugImGuiNetworkingGameThreadUpdateContextBase.h"

// engine
#include "Containers/Array.h"
#include "Misc/NetworkGuid.h"

class FKLDebugImGuiNetworkingMessage_FeatureStatusUpdate;

class KLDEBUGIMGUICLIENT_API FKLDebugImGuiClientGameThreadContext final : public FKLDebugImGuiNetworkingGameThreadUpdateContextBase
{
public:
    explicit FKLDebugImGuiClientGameThreadContext(TArray<FKLDebugImGuiClientData>& _ClientData);

    UE_NODISCARD TArray<FKLDebugImGuiClientData>& GetClientDataMutable() const;
    UE_NODISCARD const TArray<FKLDebugImGuiClientData>& GetClientData() const;

private:
    TArray<FKLDebugImGuiClientData>& mClientsData;
};

inline const TArray<FKLDebugImGuiClientData>& FKLDebugImGuiClientGameThreadContext::GetClientData() const
{
    return GetClientDataMutable();
}

inline TArray<FKLDebugImGuiClientData>& FKLDebugImGuiClientGameThreadContext::GetClientDataMutable() const
{
    return mClientsData;
}
