#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "Feature/Networking/Input/KLDebugImGuiNetworkingFeatureInputDefines.h"

// engine
#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "Serialization/Archive.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

/*
 */
class KLDEBUGIMGUIUSER_API FKLDebugImGuiNetworking_GatherDataInput final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiNetworking_GatherDataInput(const UWorld& _World, const EKLDebugImGuiNetworkingEnviroment _Enviroment, UObject& _OwnerObject, FKLDebugImGuiFeatureContext_Base* _ContextData, FArchive& _Archive);

    UE_NODISCARD EKLDebugImGuiNetworkingEnviroment GetCurrentEnviroment() const;
    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD const UObject& GetOwnerObject() const;
    UE_NODISCARD UObject& GetOwnerObjectMutable() const;
    UE_NODISCARD FArchive& GetArchiveMutable() const;

    //the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    //It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template<typename ContextType>
    UE_NODISCARD ContextType& GetContextMutable() const;

    //the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    //It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template<typename ContextType>
    UE_NODISCARD const ContextType& GetContext() const;

private:
    UObject& mOwnerObject;
    const UWorld& mWorld;
    FKLDebugImGuiFeatureContext_Base* mContextData = nullptr;
    FArchive& mArchive;
    EKLDebugImGuiNetworkingEnviroment mCurrentEnviroment = EKLDebugImGuiNetworkingEnviroment::Count;
};

inline EKLDebugImGuiNetworkingEnviroment FKLDebugImGuiNetworking_GatherDataInput::GetCurrentEnviroment() const
{
    return mCurrentEnviroment;
}

inline UObject& FKLDebugImGuiNetworking_GatherDataInput::GetOwnerObjectMutable() const
{
    return mOwnerObject;
}

inline const UObject& FKLDebugImGuiNetworking_GatherDataInput::GetOwnerObject() const
{
    return GetOwnerObjectMutable();
}

inline const UWorld& FKLDebugImGuiNetworking_GatherDataInput::GetWorld() const
{
    return mWorld;
}

inline FArchive& FKLDebugImGuiNetworking_GatherDataInput::GetArchiveMutable() const
{
    return mArchive;
}

template<typename ContextType>
inline ContextType& FKLDebugImGuiNetworking_GatherDataInput::GetContextMutable() const
{
    checkf(mContextData && mContextData->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContextData);
}

template<typename ContextType>
inline const ContextType& FKLDebugImGuiNetworking_GatherDataInput::GetContext() const
{
    return GetContextMutable<ContextType>();
}