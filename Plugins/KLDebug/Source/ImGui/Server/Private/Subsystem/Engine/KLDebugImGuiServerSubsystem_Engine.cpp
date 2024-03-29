// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Subsystem/Engine/KLDebugImGuiServerSubsystem_Engine.h"

#include "Server/KLDebugImGuiTCPServerGameThreadContext.h"

// modules
#include "ImGui/Framework/Public/Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "ImGui/Framework/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "ImGui/Networking/Public/Commands/Manager/KLDebugNetworkingCommandManager.h"
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceBase.h"
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceTypes.h"
#include "Networking/Arbitrer/Public/Luncher/KLDebugNetworkingArbitrerLuncher.h"

// engine
#include "Engine/Engine.h"
#include "Engine/World.h"

#include "Kismet/KismetSystemLibrary.h"

UKLDebugImGuiServerSubsystem_Engine* UKLDebugImGuiServerSubsystem_Engine::TryGetMutable()
{
    if (GEngine)
    {
        return GEngine->GetEngineSubsystem<UKLDebugImGuiServerSubsystem_Engine>();
    }

    return nullptr;
}

void UKLDebugImGuiServerSubsystem_Engine::Initialize(FSubsystemCollectionBase& _Collection)
{
    Super::Initialize(_Collection);

    KL::Debug::Networking::Commands::Manager::Init();
    mServerConnection.InitSocket(TEXT("KLDebugImguiServerThread"));

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();

#if !WITH_EDITOR
    CookedOnly_InitFeatureMapIfNeeded();
#endif
}

void UKLDebugImGuiServerSubsystem_Engine::Deinitialize()
{
    Super::Deinitialize();

    KL::Debug::Networking::Arbitrer::TryCloseArbitrer();
    mServerConnection.ClearConnection();
}

bool UKLDebugImGuiServerSubsystem_Engine::IsValidWorld(UWorld& _World) const
{
    if (!Super::IsValidWorld(_World))
    {
        return false;
    }

    const bool IsServer = UKismetSystemLibrary::IsDedicatedServer(&_World);
    return IsServer;
}

void UKLDebugImGuiServerSubsystem_Engine::Tick(float _DeltaTime)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiServerSubsystemEngine_Tick);

    LunchArbitrerIfNeeded();

    FKLDebugImGuiTCPServerGameThreadContext Context;
    GatherUpdateData(Context);

    UKLDebugImGuiServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::TryGetMutable();
    mServerConnection.GetConnectionMutable().TickGameThread(Context);
    mShouldTick = Context.GetShouldKeepTicking();
}

void UKLDebugImGuiServerSubsystem_Engine::LunchArbitrerIfNeeded()
{
    // we lunch the arbiter process only if we have world which make use of the server/client architecture
    // After that we keep the process alive till the Unreal process itself stops (so we don't start a process up
    // each time the user wants press Play in editor for example)

    if (mHasTryToLunchArbitrer || !HasValidWorlds())
    {
        return;
    }

    mHasTryToLunchArbitrer = true;
    static_cast<void>(KL::Debug::Networking::Arbitrer::TryLunchArbitrer());
    if (KL::Debug::Networking::Arbitrer::IsArbitrerRunning())
    {
        mServerConnection.GetConnectionMutable().InitArbitrer();
    }
}

#if !WITH_EDITOR

void UKLDebugImGuiServerSubsystem_Engine::CookedOnly_InitFeatureMapIfNeeded()
{
    // this is needed only in package builds as the client and server in that case can be out of sync
    // in the sense that we can have a deployed server which was built with a
    // different version then the one of the client. The Features indexes in this case could not match anymore.
    // so make a pass here once so we store
    // NOTE: we use the imgui path as key as we dont expect any collisions on those

    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiServerSubsystemEngine_CookedOnly_InitFeatureMapIfNeeded);

    UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::GetMutable();
    check(ImGuiEngineSubsystem != nullptr);
    const FKLDebugImGuiFeaturesTypesContainerManager& FeatureContainerManager = ImGuiEngineSubsystem->GetFeatureContainerManager();

    uint32 OverallSize = 0;
    for (int32 i = 0; i < static_cast<int32>(EImGuiInterfaceType::COUNT); ++i)
    {
        const EImGuiInterfaceType ContainerType = static_cast<EImGuiInterfaceType>(i);
        const FKLDebugImGuiFeatureContainerBase& Container = FeatureContainerManager.GetContainer(ContainerType);
        OverallSize += Container.GetFeaturesCount();
    }

    mCookOnly_FeatureToContainerIndex.Reserve(OverallSize);

    for (int32 i = 0; i < static_cast<int32>(EImGuiInterfaceType::COUNT); ++i)
    {
        const EImGuiInterfaceType ContainerType = static_cast<EImGuiInterfaceType>(i);
        const FKLDebugImGuiFeatureContainerBase& Container = FeatureContainerManager.GetContainer(ContainerType);
        FKLDebugImGuiFeaturesConstIterator FeatureIterator = Container.GetFeaturesConstIterator();
        for (; FeatureIterator; ++FeatureIterator)
        {
            const IKLDebugFeatureInterfaceBase& FeatureInterface = FeatureIterator.GetFeatureInterfaceCasted<IKLDebugFeatureInterfaceBase>();
            ensureMsgf(mCookOnly_FeatureToContainerIndex.Find(FeatureInterface.GetImGuiPath()) == nullptr, TEXT("colliding key found"));
            mCookOnly_FeatureToContainerIndex.Emplace(FeatureInterface.GetImGuiPath(), FeatureIterator.GetFeatureDataIndex());
        }
    }
}

TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex> UKLDebugImGuiServerSubsystem_Engine::CookedOnly_TryGetFeatureFromName(const FName& _FeaturePath) const
{
    if (const KL::Debug::ImGui::Features::Types::FeatureIndex* FeatureIndex = mCookOnly_FeatureToContainerIndex.Find(_FeaturePath))
    {
        return *FeatureIndex;
    }

    return TOptional<KL::Debug::ImGui::Features::Types::FeatureIndex>();
}

#endif