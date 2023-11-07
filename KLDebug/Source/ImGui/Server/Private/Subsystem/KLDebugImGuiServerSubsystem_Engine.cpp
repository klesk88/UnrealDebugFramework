#include "Subsystem/KLDebugImGuiServerSubsystem_Engine.h"

#include "Server/KLDebugImGuiTCPServerGameThreadContext.h"

//modules
#include "ImGui/Framework/Public/Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "ImGui/Framework/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"

//engine
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
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

    mServerConnection.InitSocket(TEXT("KLDebugImguiServerThread"));

    FGameModeEvents::OnGameModePostLoginEvent().AddUObject(this, &UKLDebugImGuiServerSubsystem_Engine::OnGameModePostLoginEventHandler);
    FGameModeEvents::OnGameModeLogoutEvent().AddUObject(this, &UKLDebugImGuiServerSubsystem_Engine::OnGameModePostLogoutEventHandle);

    mConnectedPlayer.Reserve(10);
    mDisconnectedPlayers.Reserve(10);

#if !WITH_EDITOR
    CookedOnly_InitFeatureMapIfNeeded();
#endif
}

void UKLDebugImGuiServerSubsystem_Engine::Deinitialize()
{
    Super::Deinitialize();

    mServerConnection.ClearConnection();
    FGameModeEvents::OnGameModePostLoginEvent().RemoveAll(this);
    FGameModeEvents::OnGameModeLogoutEvent().RemoveAll(this);
}

bool UKLDebugImGuiServerSubsystem_Engine::IsValidWorld(UWorld& _World) const
{
    if (!Super::IsValidWorld(_World))
    {
        return false;
    }

    const bool IsServer = UKismetSystemLibrary::IsServer(&_World);
    return IsServer;
}

void UKLDebugImGuiServerSubsystem_Engine::OnGameModePostLoginEventHandler(AGameModeBase* _GameMode, APlayerController* _NewPlayer)
{
    mConnectedPlayer.Emplace(_NewPlayer);
    SetShouldTick();
}

void UKLDebugImGuiServerSubsystem_Engine::OnGameModePostLogoutEventHandle(AGameModeBase* _GameMode, AController* _Exiting)
{
    mDisconnectedPlayers.Emplace(FObjectKey(_Exiting));
    SetShouldTick();
}

void UKLDebugImGuiServerSubsystem_Engine::Tick(float _DeltaTime)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiServerSubsystemEngine_Tick);

    FKLDebugImGuiTCPServerGameThreadContext Context{ mConnectedPlayer, mDisconnectedPlayers };

    UKLDebugImGuiServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::TryGetMutable();
    mServerConnection.GetConnectionMutable().TickGameThread(Context);
    mShouldTick = Context.GetShouldKeepTicking();
}

#if !WITH_EDITOR

void UKLDebugImGuiServerSubsystem_Engine::CookedOnly_InitFeatureMapIfNeeded()
{
    //this is needed only in package builds as the client and server in that case can be out of sync 
    //in the sense that we can have a deployed server which was built with a
    //different version then the one of the client. The Features indexes in this case could not match anymore.
    //so make a pass here once so we store 
    //NOTE: we use the imgui path as key as we dont expect any collisions on those

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
            const IKLDebugImGuiFeatureInterfaceBase& FeatureInterface = FeatureIterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
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