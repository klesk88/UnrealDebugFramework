#include "Networking/Server/KLDebugImGuiNetworkServerSubsystem.h"

//engine
#include "Kismet/KismetSystemLibrary.h"

bool UKLDebugImGuiNetworkServerSubsystem::ShouldCreateSubsystem(UObject* _Outer) const
{
#if WITH_SERVER_CODE
    return true;
#else
    return false;
#endif
}

void UKLDebugImGuiNetworkServerSubsystem::Initialize(FSubsystemCollectionBase& _Collection)
{
    Super::Initialize(_Collection);

    mOnWorldPostInitializeDelegateHandle = FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UKLDebugImGuiNetworkServerSubsystem::OnWorldPostInitialize);
}

void UKLDebugImGuiNetworkServerSubsystem::Deinitialize()
{
    Super::Deinitialize();

    if (mOnWorldPostInitializeDelegateHandle.IsValid())
    {
        FWorldDelegates::OnPostWorldInitialization.Remove(mOnWorldPostInitializeDelegateHandle);
        mOnWorldPostInitializeDelegateHandle.Reset();
    }

    mServer.Clear();
}

void UKLDebugImGuiNetworkServerSubsystem::OnWorldPostInitialize(UWorld* _World, [[maybe_unused]] const UWorld::InitializationValues _IVS)
{
    if (!_World)
    {
        return;
    }

    if (!UKismetSystemLibrary::IsServer(_World) || UKismetSystemLibrary::IsStandalone(_World))
    {
        return;
    }

    if (mServer.IsSocketRegistered())
    {
        return;
    }

    mServer.Init(*_World);
}
