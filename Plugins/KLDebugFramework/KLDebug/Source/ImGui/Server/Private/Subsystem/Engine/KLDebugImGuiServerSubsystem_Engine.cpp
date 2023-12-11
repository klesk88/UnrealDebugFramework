// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Subsystem/Engine/KLDebugImGuiServerSubsystem_Engine.h"

#include "Server/KLDebugImGuiTCPServerGameThreadContext.h"

// modules
#include "ImGui/Framework/Public/Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "ImGui/Framework/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"
#include "Networking/Arbitrer/Public/Definitions/KLDebugNetworkingArbitrerDefinitions.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"

// engine
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/KismetSystemLibrary.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include <Windows.h>
#endif

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

    KL::Debug::Networking::Message::InitHeaderSize();

    mServerConnection.InitSocket(TEXT("KLDebugImguiServerThread"));

    TryLunchArbitrer();

#if !WITH_EDITOR
    CookedOnly_InitFeatureMapIfNeeded();
#endif
}

void UKLDebugImGuiServerSubsystem_Engine::Deinitialize()
{
    Super::Deinitialize();

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

    FKLDebugImGuiTCPServerGameThreadContext Context;
    GatherUpdateData(Context);

    UKLDebugImGuiServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::TryGetMutable();
    mServerConnection.GetConnectionMutable().TickGameThread(Context);
    mShouldTick = Context.GetShouldKeepTicking();
}

void UKLDebugImGuiServerSubsystem_Engine::TryLunchArbitrer()
{
#if PLATFORM_WINDOWS
    // based on TryAutoConnect in TraceAucilary.cpp

    // If we can detect a named event it means the arbitrer is running.
    HANDLE KnownEvent = ::OpenEvent(EVENT_ALL_ACCESS, false, *KL::Debug::Networking::Arbitrer::ArbitrerSessionEvent);
    if (KnownEvent == nullptr)
    {
        LunchArbitrer();
    }
#elif PLATFORM_UNIX
    LunchArbitrer();
#endif
}

#if PLATFORM_WINDOWS
void UKLDebugImGuiServerSubsystem_Engine::LunchArbitrer()
{
    TWideStringBuilder<MAX_PATH + 32> CreateProcArgs;
    CreateProcArgs << IPluginManager::Get().FindPlugin("KLDebug")->GetBaseDir();
    CreateProcArgs << TEXT("/Resources/Arbitrer/KLDebugArbitrer.exe");

    uint32 CreateProcFlags = CREATE_BREAKAWAY_FROM_JOB | CREATE_NEW_CONSOLE;
    STARTUPINFOW StartupInfo = { sizeof(STARTUPINFOW) };
    PROCESS_INFORMATION ProcessInfo = {};
    BOOL bOk = CreateProcessW(LPWSTR(*CreateProcArgs), nullptr, nullptr, nullptr, false, CreateProcFlags, nullptr, nullptr, &StartupInfo, &ProcessInfo);

    if (!bOk)
    {
        UE_LOG(LogKLDebug_Networking, Warning, TEXT("UKLDebugImGuiServerSubsystem_Engine::LunchArbitrer>> unable to launch arbitrer with '%s' (%08x)"), *CreateProcArgs, GetLastError());
        return;
    }

    CloseHandle(ProcessInfo.hProcess);
    CloseHandle(ProcessInfo.hThread);
}
#elif PLATFORM_UNIX
void UKLDebugImGuiServerSubsystem_Engine::LunchArbitrer()
{
}
#endif

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