#pragma once

#include "Server/KLDebugImGuiServerManager.h"

//modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Map.h"
#include "Misc/Optional.h"
#include "Subsystems/EngineSubsystem.h"
#include "UObject/NameTypes.h"

#include "KLDebugImGuiServerSubsystem_Engine.generated.h"

class UWorld;

UCLASS(Transient)
class KLDEBUGIMGUISERVER_API UKLDebugImGuiServerSubsystem_Engine final : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    //UEngineSubsystem
    bool ShouldCreateSubsystem(UObject* _Outer) const final;
    void Initialize(FSubsystemCollectionBase& _Collection) final;
    void Deinitialize() final;
    //UEngineSubsystem

    UE_NODISCARD static UKLDebugImGuiServerSubsystem_Engine* GetMutable();
    UE_NODISCARD static const UKLDebugImGuiServerSubsystem_Engine* Get();

    void InitServerFromWorld(UWorld& _World);
    void ClearServerFromWorld(const UWorld& _World);

private:
    FKLDebugImGuiServerManager mServer;

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

inline const UKLDebugImGuiServerSubsystem_Engine* UKLDebugImGuiServerSubsystem_Engine::Get()
{
    return GetMutable();
}

inline void UKLDebugImGuiServerSubsystem_Engine::ClearServerFromWorld(const UWorld& _World)
{
    mServer.ClearFromWorld(_World);
}
