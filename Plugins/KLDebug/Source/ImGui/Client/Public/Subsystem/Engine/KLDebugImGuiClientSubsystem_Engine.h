// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Client/KLDebugImGuiNetworkingTCPClient.h"

// modules
#include "ImGui/Networking/Public/Subsystem/Engine/KLDebugImGuiNetworkingSubsystem_EngineBase.h"
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingConnectionRunnableContainer.h"

// engine
#include "Containers/Array.h"
#include "Subsystems/EngineSubsystem.h"

#include "KLDebugImGuiClientSubsystem_Engine.generated.h"

class UKLDebugImGuiWorldSubsystem;
class FKLDebugImGuiFeatureStatusUpdateData;

UCLASS(Transient)
class KLDEBUGIMGUICLIENT_API UKLDebugImGuiClientSubsystem_Engine final : public UKLDebugImGuiNetworkingSubsystem_EngineBase
{
    GENERATED_BODY()

public:
    // UEngineSubsystem
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;
    // UEngineSubsystem

    static const UKLDebugImGuiClientSubsystem_Engine* TryGet();
    static UKLDebugImGuiClientSubsystem_Engine* TryGetMutable();

private:
    // UKLDebugImGuiNetworkingSubsystem_EngineBase
    [[nodiscard]] FKLDebugImGuiNetworkingTCPBase* GetConnectionMutable() final;
    [[nodiscard]] const FKLDebugImGuiNetworkingTCPBase* GetConnection() const final;
    [[nodiscard]] bool IsValidWorld(UWorld& _World) const final;
    void OnImGuiSusbsytemAdded(UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem, UWorld& _World) final;

    ETickableTickType GetTickableTickTypeChild() const;
    TStatId GetStatId() const final;
    void Tick(float _DeltaTime) final;
    // UKLDebugImGuiNetworkingSubsystem_EngineBase

private:
    TKLDebugImGuiNetworkingConnectionRunnableContainer<FKLDebugImGuiNetworkingTCPClient> mClientConnection;
};

inline const UKLDebugImGuiClientSubsystem_Engine* UKLDebugImGuiClientSubsystem_Engine::TryGet()
{
    return TryGetMutable();
}

inline FKLDebugImGuiNetworkingTCPBase* UKLDebugImGuiClientSubsystem_Engine::GetConnectionMutable()
{
    return &mClientConnection.GetConnectionMutable();
}

inline const FKLDebugImGuiNetworkingTCPBase* UKLDebugImGuiClientSubsystem_Engine::GetConnection() const
{
    return &mClientConnection.GetConnection();
}

inline TStatId UKLDebugImGuiClientSubsystem_Engine::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UKLDebugImGuiClientSubsystem_Engine, STATGROUP_Tickables);
}

inline ETickableTickType UKLDebugImGuiClientSubsystem_Engine::GetTickableTickTypeChild() const
{
    return ETickableTickType::Conditional;
}
