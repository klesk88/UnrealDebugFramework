#pragma once

//imgui module
#include "ImGui/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Map.h"
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

private:
    TMap<FName, KL::Debug::ImGui::Features::Types::FeatureIndex> mFeatureToContainerIndex;
};

inline const UKLDebugImGuiNetworkingServerSubsystem_Engine* UKLDebugImGuiNetworkingServerSubsystem_Engine::Get()
{
    return GetMutable();
}