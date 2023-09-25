#pragma once

#include "Server/KLDebugImGuiNetworkingManager_Server.h"

//imgui module
#include "ImGui/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Map.h"
#include "Misc/Optional.h"
#include "Subsystems/EngineSubsystem.h"
#include "UObject/NameTypes.h"

#include "KLDebugImGuiNetworkingServerSubsystem_Engine.generated.h"

class UWorld;

UCLASS(Transient)
class KLDEBUGIMGUINETWORKINGSERVER_API UKLDebugImGuiNetworkingServerSubsystem_Engine final : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    bool ShouldCreateSubsystem(UObject* _Outer) const final;
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;

    UE_NODISCARD static UKLDebugImGuiNetworkingServerSubsystem_Engine* GetMutable();
    UE_NODISCARD static const UKLDebugImGuiNetworkingServerSubsystem_Engine* Get();

    void InitServerFromWorld(UWorld& _World);
    void ClearServerFromWorld(const UWorld& _World);

private:
    FKLDebugImGuiNetworkingManager_Server mServer;

#if !WITH_EDITOR
public:
    TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> CookedOnly_TryGetFeatureFromName(const FName& _FeaturePath) const;

private:
    void CookedOnly_InitFeatureMapIfNeeded();

private:
    TMap<FName, KL::Debug::ImGui::Features::Types::FeatureIndex> mCookOnly_FeatureToContainerIndex;
    bool mCookOnly_MapInitialized = false;
#endif
};

inline const UKLDebugImGuiNetworkingServerSubsystem_Engine* UKLDebugImGuiNetworkingServerSubsystem_Engine::Get()
{
    return GetMutable();
}

inline void UKLDebugImGuiNetworkingServerSubsystem_Engine::ClearServerFromWorld(const UWorld& _World)
{
    mServer.ClearFromWorld(_World);
}
