//#include "Subsystem/KLDebugImGuiServerSubsystem_World.h"
//
//#include "Server/KLDebugImGuiTCPServerGameThreadContext.h"
//#include "Subsystem/KLDebugImGuiServerSubsystem_Engine.h"
//
////engine
//#include "Engine/Engine.h"
//#include "Engine/World.h"
//#include "GameFramework/GameModeBase.h"
//#include "GameFramework/PlayerController.h"
//#include "Kismet/GameplayStatics.h"
//#include "Kismet/KismetSystemLibrary.h"
//
//UKLDebugImGuiServerSubsystem_World* UKLDebugImGuiServerSubsystem_World::TryGetMutable(const UObject& _Object)
//{
//    const UWorld* World = GEngine->GetWorldFromContextObject(&_Object, EGetWorldErrorMode::ReturnNull);
//    return World ? World->GetSubsystem<UKLDebugImGuiServerSubsystem_World>() : nullptr;
//}
//
//bool UKLDebugImGuiServerSubsystem_World::ShouldCreateSubsystem(UObject* _Outer) const
//{
//#if WITH_SERVER_CODE && !WITH_EDITOR
//    return true;
//#else
//    const UWorld* World = Cast<const UWorld>(_Outer);
//    checkf(World != nullptr, TEXT("world must be valid"));
//    return IsValid(*World);
//#endif
//}
//
//bool UKLDebugImGuiServerSubsystem_World::DoesSupportWorldType(const EWorldType::Type _WorldType) const
//{
//    switch (_WorldType)
//    {
//
//    case EWorldType::Type::Game:
//    case EWorldType::Type::Editor:
//    case EWorldType::Type::PIE:
//    case EWorldType::Type::GameRPC:
//        return true;
//    case EWorldType::Type::GamePreview:
//    case EWorldType::Type::EditorPreview:
//    case EWorldType::Type::None:
//    case EWorldType::Type::Inactive:
//        return false;
//    default:
//        ensureMsgf(false, TEXT("case not handled"));
//        return false;
//    }
//}
//
//void UKLDebugImGuiServerSubsystem_World::Initialize(FSubsystemCollectionBase& _Collection)
//{
//    Super::Initialize(_Collection);
//}
//
//void UKLDebugImGuiServerSubsystem_World::Deinitialize()
//{
//    Super::Deinitialize();
//
//    if (UKLDebugImGuiServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::TryGetMutable())
//    {
//        //ServerEngineSubsystem->OnWorldChange(nullptr);
//    }
//
//    if (mOnGameModePostLoginDelegateHandle.IsValid())
//    {
//        FGameModeEvents::OnGameModePostLoginEvent().Remove(mOnGameModePostLoginDelegateHandle);
//        mOnGameModePostLoginDelegateHandle.Reset();
//    }
//
//    if (mOnGameModePostLogoutDelegateHandle.IsValid())
//    {
//        FGameModeEvents::OnGameModeLogoutEvent().Remove(mOnGameModePostLogoutDelegateHandle);
//        mOnGameModePostLogoutDelegateHandle.Reset();
//    }
//}
//
//void UKLDebugImGuiServerSubsystem_World::OnWorldBeginPlay(UWorld& _World)
//{
//    //we can reach here because the world was not yet initialized in editor. so double check at this point that we are
//    //really on a client
//    if (!IsValid(_World))
//    {
//        return;
//    }
//
//    mOnGameModePostLoginDelegateHandle = FGameModeEvents::OnGameModePostLoginEvent().AddUObject(this, &UKLDebugImGuiServerSubsystem_World::OnGameModePostLoginEventHandler);
//    mOnGameModePostLogoutDelegateHandle = FGameModeEvents::OnGameModeLogoutEvent().AddUObject(this, &UKLDebugImGuiServerSubsystem_World::OnGameModePostLogoutEventHandle);
//
//    AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
//    ensureMsgf(!GameMode || GameMode->GetNumPlayers() == 0, TEXT("we dont expect players to be already connected"));
//
//    UKLDebugImGuiServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::TryGetMutable();
//    //ServerEngineSubsystem->OnWorldChange(&_World);
//}
//
//bool UKLDebugImGuiServerSubsystem_World::IsValid(const UWorld& _World) const
//{
//    const bool IsServer = UKismetSystemLibrary::IsServer(&_World);
//    return IsServer;
//}
//
//void UKLDebugImGuiServerSubsystem_World::OnGameModePostLoginEventHandler(AGameModeBase* _GameMode, APlayerController* _NewPlayer)
//{
//    mConnectedPlayer.Emplace(_NewPlayer);
//    mShouldTick = true;
//}
//
//void UKLDebugImGuiServerSubsystem_World::OnGameModePostLogoutEventHandle(AGameModeBase* _GameMode, AController* _Exiting)
//{
//    mDisconnectedPlayers.Emplace(FObjectKey(_Exiting));
//    mShouldTick = true;
//}
//
//void UKLDebugImGuiServerSubsystem_World::Tick(float DeltaTime)
//{
//    FKLDebugImGuiTCPServerGameThreadContext Context{ *GetWorld(), mConnectedPlayer, mDisconnectedPlayers };
//
//    UKLDebugImGuiServerSubsystem_Engine* ServerEngineSubsystem = UKLDebugImGuiServerSubsystem_Engine::TryGetMutable();
//   // ServerEngineSubsystem->TickOnGameThread(Context);
//    mShouldTick = Context.GetShouldKeepTicking();
//}
//
