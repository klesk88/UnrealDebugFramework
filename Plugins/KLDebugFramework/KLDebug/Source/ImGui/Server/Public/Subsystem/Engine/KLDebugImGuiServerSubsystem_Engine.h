// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Server/KLDebugImGuiNetworkingTCPServer.h"
#include "Server/KLDebugImGuiServerConnectionDefinitions.h"

// modules
#include "ImGui/Networking/Public/Subsystem/Engine/KLDebugImGuiNetworkingSubsystem_EngineBase.h"
#include "ImGui/Networking/Public/TCP/KLDebugImGuiNetworkingConnectionRunnableContainer.h"

// engine
#include "Containers/Array.h"
#include "Engine/EngineTypes.h"
#include "Subsystems/EngineSubsystem.h"
#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#if !WITH_EDITOR

// modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Map.h"
#include "Misc/Optional.h"
#include "UObject/NameTypes.h"

#endif    //! WITH_EDITOR

#include "KLDebugImGuiServerSubsystem_Engine.generated.h"

class UWorld;

UCLASS(Transient)
class KLDEBUGIMGUISERVER_API UKLDebugImGuiServerSubsystem_Engine final : public UKLDebugImGuiNetworkingSubsystem_EngineBase
{
    GENERATED_BODY()

public:
    // UEngineSubsystem
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;
    // UEngineSubsystem

    UE_NODISCARD static UKLDebugImGuiServerSubsystem_Engine* TryGetMutable();
    UE_NODISCARD static const UKLDebugImGuiServerSubsystem_Engine* TryGet();

private:
    // UKLDebugImGuiNetworkingSubsystem_EngineBase
    UE_NODISCARD FKLDebugImGuiNetworkingTCPBase* GetConnectionMutable() final;
    UE_NODISCARD const FKLDebugImGuiNetworkingTCPBase* GetConnection() const final;
    UE_NODISCARD bool IsValidWorld(UWorld& _World) const final;

    ETickableTickType GetTickableTickTypeChild() const final;
    TStatId GetStatId() const final;
    void Tick(float _DeltaTime) final;
    // UKLDebugImGuiNetworkingSubsystem_EngineBase

    void LunchArbitrerIfNeeded();

private:
    // NOTE: if we change the ownership of the connection, then we need to update also FKLDebugImGuiNetworkingTCPServer::TickCachedConnections
    // where we request the tick to make sure is thread safe
    TKLDebugImGuiNetworkingConnectionRunnableContainer<FKLDebugImGuiNetworkingTCPServer> mServerConnection;
    bool mHasTryToLunchArbitrer = false;

#if !WITH_EDITOR
public:
    TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> CookedOnly_TryGetFeatureFromName(const FName& _FeaturePath) const;

private:
    void CookedOnly_InitFeatureMapIfNeeded();

private:
    TMap<FName, KL::Debug::ImGui::Features::Types::FeatureIndex> mCookOnly_FeatureToContainerIndex;
#endif
};

inline const UKLDebugImGuiServerSubsystem_Engine* UKLDebugImGuiServerSubsystem_Engine::TryGet()
{
    return TryGetMutable();
}

inline FKLDebugImGuiNetworkingTCPBase* UKLDebugImGuiServerSubsystem_Engine::GetConnectionMutable()
{
    return &mServerConnection.GetConnectionMutable();
}

inline const FKLDebugImGuiNetworkingTCPBase* UKLDebugImGuiServerSubsystem_Engine::GetConnection() const
{
    return &mServerConnection.GetConnection();
}

inline TStatId UKLDebugImGuiServerSubsystem_Engine::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UKLDebugImGuiServerSubsystem_Engine, STATGROUP_Tickables);
}

inline ETickableTickType UKLDebugImGuiServerSubsystem_Engine::GetTickableTickTypeChild() const
{
    return ETickableTickType::Conditional;
}