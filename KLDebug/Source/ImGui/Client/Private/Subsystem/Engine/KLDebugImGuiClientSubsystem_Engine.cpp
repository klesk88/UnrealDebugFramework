#include "Subsystem/Engine/KLDebugImGuiClientSubsystem_Engine.h"

#include "Client/KLDebugImGuiClientGameThreadContext.h"

//modules
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeaturesDelegates.h"
#include "ImGui/Framework/Public/Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"

//engine
#include "Engine/EngineBaseTypes.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

void UKLDebugImGuiClientSubsystem_Engine::Initialize(FSubsystemCollectionBase& _Collection)
{
    Super::Initialize(_Collection);

    mClientConnection.InitSocket(TEXT("KLDebugImguiClientThread"));
    mClientsData.Reserve(10);}

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
    FOnImGuiFeatureStateUpdated::FDelegate FeatureUpdateDelagate = FOnImGuiFeatureStateUpdated::FDelegate::CreateUObject(this, &UKLDebugImGuiClientSubsystem_Engine::OnFeatureUpdate);
    const FDelegateHandle Handle = _ImGuiSubsystem.BindOnImGuiFeatureStateUpdated(FeatureUpdateDelagate);

    ensureMsgf(mClientsData.FindByKey(FObjectKey(&_World)) == nullptr, TEXT("we should not have already one"));
    mClientsData.Emplace(_World, Handle);
}

void UKLDebugImGuiClientSubsystem_Engine::OnImGuiSusbsytemRemoved(UKLDebugImGuiWorldSubsystem& _ImGuiSubsystem, UWorld& _World)
{
    const int32 Index = mClientsData.IndexOfByKey(FObjectKey(&_World));
    if (Index == INDEX_NONE)
    {
        ensureMsgf(false, TEXT("should be in list"));
        return;
    }

    mClientsData[Index].Shutdown(_ImGuiSubsystem);
    mClientsData.RemoveAtSwap(Index, 1, false);
}

void UKLDebugImGuiClientSubsystem_Engine::OnFeatureUpdate(const FKLDebugImGuiFeatureStatusUpdateData& _FeatureUpdateData)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientSubsystemEngine_OnFeatureUpdate);

    const UWorld& World = _FeatureUpdateData.GetOwnerWorld();
    FKLDebugImGuiClientData* ClientData = mClientsData.FindByKey(FObjectKey(&World));
    if (!ClientData)
    {
        ensureMsgf(false, TEXT("this should never be possible"));
        return;
    }

    mShouldTick = ClientData->OnFeatureUpdate(_FeatureUpdateData);
}

void UKLDebugImGuiClientSubsystem_Engine::Tick(float _DeltaTime)
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiClientSubsystemEngine_Tick);

    FKLDebugImGuiClientGameThreadContext Context{ mClientsData };
    GatherUpdateData(Context);
    mClientConnection.GetConnectionMutable().TickGameThread(Context);
    mShouldTick = Context.GetShouldKeepTicking();
}
