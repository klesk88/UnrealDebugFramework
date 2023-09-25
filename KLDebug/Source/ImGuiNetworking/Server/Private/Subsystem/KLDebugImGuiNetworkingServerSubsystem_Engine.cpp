#include "Subsystem/KLDebugImGuiNetworkingServerSubsystem_Engine.h"

//engine
#include "Engine/Engine.h"

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
#if WITH_SERVER_CODE
    return true;
#else
    return false;
#endif
}

void UKLDebugImGuiNetworkingServerSubsystem_Engine::Initialize(FSubsystemCollectionBase& _Collection)
{
    Super::Initialize(_Collection);
}

void UKLDebugImGuiNetworkingServerSubsystem_Engine::Deinitialize()
{
    Super::Deinitialize();

}