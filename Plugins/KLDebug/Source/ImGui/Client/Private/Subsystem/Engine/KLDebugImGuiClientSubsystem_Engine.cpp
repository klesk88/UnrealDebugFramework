// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Subsystem/Engine/KLDebugImGuiClientSubsystem_Engine.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"

// modules
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"
#include "ImGui/Networking/Public/Commands/Manager/KLDebugNetworkingCommandManager.h"

// engine
#include "Engine/Engine.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

void UKLDebugImGuiClientSubsystem_Engine::Initialize(FSubsystemCollectionBase& _Collection)
{
    Super::Initialize(_Collection);

    KL::Debug::Networking::Commands::Manager::Init();
    mClientConnection.InitSocket(TEXT("KLDebugImguiClientThread"));
}

void UKLDebugImGuiClientSubsystem_Engine::Deinitialize()
{
    Super::Deinitialize();

    mClientConnection.ClearConnection();
}

UKLDebugImGuiClientSubsystem_Engine* UKLDebugImGuiClientSubsystem_Engine::TryGetMutable()
{
    return GEngine->GetEngineSubsystem<UKLDebugImGuiClientSubsystem_Engine>();
}

bool UKLDebugImGuiClientSubsystem_Engine::IsValidWorld(UWorld& _World) const
{
    if (!Super::IsValidWorld(_World))
    {
        return false;
    }

    const UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = _World.GetSubsystem<UKLDebugImGuiWorldSubsystem>();
    if (!ImGuiWorldSubsystem)
    {
        return false;
    }

    const bool IsClient = !UKismetSystemLibrary::IsServer(&_World);
    return IsClient;
}

void UKLDebugImGuiClientSubsystem_Engine::OnImGuiSusbsytemAdded(UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem, UWorld& _World)
{
    // add the client to the list of connections
    SetShouldTick();
}

void UKLDebugImGuiClientSubsystem_Engine::Tick(float _DeltaTime)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientSubsystemEngine_Tick);

    FKLDebugImGuiClientGameThreadContext Context{};
    GatherUpdateData(Context);
    mClientConnection.GetConnectionMutable().TickGameThread(Context);
    mShouldTick = Context.GetShouldKeepTicking();
}
