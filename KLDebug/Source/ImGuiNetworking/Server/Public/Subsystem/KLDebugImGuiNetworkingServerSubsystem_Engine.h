#pragma once

//imgui module
#include "ImGui/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Map.h"
#include "Misc/Optional.h"
#include "Subsystems/EngineSubsystem.h"
#include "UObject/NameTypes.h"

#include "KLDebugImGuiNetworkingServerSubsystem_Engine.generated.h"

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

#if !WITH_EDITOR
public:
    void CookedOnly_InitFeatureMapIfNeeded();
    TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> CookedOnly_TryGetFeatureFromName(const FName& _FeaturePath) const;

private:
    TMap<FName, KL::Debug::ImGui::Features::Types::FeatureIndex> mCookOnly_FeatureToContainerIndex;
    bool mCookOnly_MapInitialized = false;
#endif
};

inline const UKLDebugImGuiNetworkingServerSubsystem_Engine* UKLDebugImGuiNetworkingServerSubsystem_Engine::Get()
{
    return GetMutable();
}