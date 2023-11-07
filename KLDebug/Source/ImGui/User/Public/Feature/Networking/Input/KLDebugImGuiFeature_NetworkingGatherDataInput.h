#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "Feature/Networking/Input/KLDebugImGuiFeature_NetworkingInputDefines.h"

// engine
#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "Serialization/Archive.h"
#include "Templates/EnableIf.h"
#include "Templates/UnrealTemplate.h"
#include "Templates/UnrealTypeTraits.h"

class IKLDebugImGuiFeatureInterface_Selectable;
class UWorld;

/*
 */
class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeature_NetworkingGatherDataInput final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeature_NetworkingGatherDataInput(const UWorld& _World, const EKLDebugImGuiNetworkingEnviroment _Enviroment, UObject* _OwnerObject, FKLDebugImGuiFeatureContext_Base* _ContextData, FArchive& _Archive);

    UE_NODISCARD EKLDebugImGuiNetworkingEnviroment GetCurrentEnviroment() const;
    UE_NODISCARD const UWorld& GetWorld() const;

    //the owner is guaranteed to be valid only for IKLDebugImGuiFeatureInterface_Selectable
    template<typename CallingClass>
    UE_NODISCARD typename TEnableIf<TIsDerivedFrom<CallingClass, IKLDebugImGuiFeatureInterface_Selectable>::IsDerived, const UObject&>::Type  TryGetOwnerObject() const;
    template<typename CallingClass>
    UE_NODISCARD typename TEnableIf<TIsDerivedFrom<CallingClass, IKLDebugImGuiFeatureInterface_Selectable>::IsDerived, UObject&>::Type TryGetOwnerObjectMutable() const;
    //the owner is guaranteed to be valid only for IKLDebugImGuiFeatureInterface_Selectable

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
    UObject* mOwnerObject;
    const UWorld& mWorld;
    FKLDebugImGuiFeatureContext_Base* mContextData = nullptr;
    FArchive& mArchive;
    EKLDebugImGuiNetworkingEnviroment mCurrentEnviroment = EKLDebugImGuiNetworkingEnviroment::Count;
};

inline EKLDebugImGuiNetworkingEnviroment FKLDebugImGuiFeature_NetworkingGatherDataInput::GetCurrentEnviroment() const
{
    return mCurrentEnviroment;
}

template<typename CallingClass>
inline typename TEnableIf<TIsDerivedFrom<CallingClass, IKLDebugImGuiFeatureInterface_Selectable>::IsDerived, UObject&>::Type FKLDebugImGuiFeature_NetworkingGatherDataInput::TryGetOwnerObjectMutable() const
{
    check(mOwnerObject != nullptr);
    return *mOwnerObject;
}

template<typename CallingClass>
inline typename TEnableIf<TIsDerivedFrom<CallingClass, IKLDebugImGuiFeatureInterface_Selectable>::IsDerived, const UObject&>::Type  FKLDebugImGuiFeature_NetworkingGatherDataInput::TryGetOwnerObject() const
{
    return TryGetOwnerObjectMutable<CallingClass>();
}

inline const UWorld& FKLDebugImGuiFeature_NetworkingGatherDataInput::GetWorld() const
{
    return mWorld;
}

inline FArchive& FKLDebugImGuiFeature_NetworkingGatherDataInput::GetArchiveMutable() const
{
    return mArchive;
}

template<typename ContextType>
inline ContextType& FKLDebugImGuiFeature_NetworkingGatherDataInput::GetContextMutable() const
{
    checkf(mContextData && mContextData->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContextData);
}

template<typename ContextType>
inline const ContextType& FKLDebugImGuiFeature_NetworkingGatherDataInput::GetContext() const
{
    return GetContextMutable<ContextType>();
}