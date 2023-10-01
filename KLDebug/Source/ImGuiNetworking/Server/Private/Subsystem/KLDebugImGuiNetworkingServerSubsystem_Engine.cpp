#include "Subsystem/KLDebugImGuiNetworkingServerSubsystem_Engine.h"

//modules
#include "ImGui/Framework/Public/Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "ImGui/Framework/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "ImGui/Framework/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"

//engine
#include "Engine/Engine.h"
#include "Engine/World.h"

UKLDebugImGuiNetworkingServerSubsystem_Engine* UKLDebugImGuiNetworkingServerSubsystem_Engine::GetMutable()
{
    if (GEngine)
    {
        return GEngine->GetEngineSubsystem<UKLDebugImGuiNetworkingServerSubsystem_Engine>();
    }

    return nullptr;
}

bool UKLDebugImGuiNetworkingServerSubsystem_Engine::ShouldCreateSubsystem(UObject* _Outer) const
{
    return true;
}

void UKLDebugImGuiNetworkingServerSubsystem_Engine::Initialize(FSubsystemCollectionBase& _Collection)
{
    Super::Initialize(_Collection);

    mServer.InitFromEngine();
}

void UKLDebugImGuiNetworkingServerSubsystem_Engine::Deinitialize()
{
    Super::Deinitialize();

    mServer.ClearFromEngine();
}

void UKLDebugImGuiNetworkingServerSubsystem_Engine::InitServerFromWorld(UWorld& _World)
{
    mServer.InitFromWorld(_World);

#if !WITH_EDITOR
    CookedOnly_InitFeatureMapIfNeeded();
#endif
}

#if !WITH_EDITOR

void UKLDebugImGuiNetworkingServerSubsystem_Engine::CookedOnly_InitFeatureMapIfNeeded()
{
    //this is needed only in package builds as the client and server in that case can be out of sync 
    //in the sense that we can have a deployed server which was built with a
    //different version then the one of the client. The Features indexes in this case could not match anymore.
    //so make a pass here once so we store 
    //NOTE: we use the imgui path as key as we dont expect any collisions on those

    QUICK_SCOPE_CYCLE_COUNTER(STAT_KLDebugImGuiNetworkingServerSubsystem_Engine_InitFeatureMapIfNeeded);

    if (mCookOnly_MapInitialized)
    {
        return;
    }

    mCookOnly_MapInitialized = true;

    UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::GetMutable();
    check(ImGuiEngineSubsystem != nullptr);
    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = ImGuiEngineSubsystem->GetFeatureContainerManager();

    uint32 OverallSize = 0;
    for (int32 i = 0; i < static_cast<int32>(EContainerType::COUNT); ++i)
    {
        const EContainerType ContainerType = static_cast<EContainerType>(i);
        const FKLDebugImGuiFeatureContainerBase& Container = FeatureContainerManager.GetContainer(ContainerType);
        OverallSize += Container.GetFeaturesCount();
    }

    mCookOnly_FeatureToContainerIndex.Reserve(OverallSize);

    for (int32 i = 0; i < static_cast<int32>(EContainerType::COUNT); ++i)
    {
        const EContainerType ContainerType = static_cast<EContainerType>(i);
        const FKLDebugImGuiFeatureContainerBase& Container = FeatureContainerManager.GetContainer(ContainerType);
        FKLDebugImGuiFeaturesConstIterator FeatureIterator = Container.GetFeaturesConstIterator();
        for (; FeatureIterator; ++FeatureIterator)
        {
            const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = FeatureIterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
            ensureMsgf(mCookOnly_FeatureToContainerIndex.Find(FeatureInterface.GetImGuiPath()) == nullptr, TEXT("colliding key found"));
            mCookOnly_FeatureToContainerIndex.Emplace(FeatureInterface.GetImGuiPath(), FeatureIterator.GetFeatureDataIndex());
        }
    }
}

TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> UKLDebugImGuiNetworkingServerSubsystem_Engine::CookedOnly_TryGetFeatureFromName(const FName& _FeaturePath) const
{
    if (const KL::Debug::ImGui::Features::Types::FeatureIndex* FeatureIndex = mCookOnly_FeatureToContainerIndex.Find(_FeaturePath))
    {
        return *FeatureIndex;
    }

    return TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex>();
}

#endif