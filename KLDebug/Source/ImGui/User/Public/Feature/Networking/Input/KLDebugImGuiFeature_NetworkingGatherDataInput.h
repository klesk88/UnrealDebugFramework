// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"

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
    explicit FKLDebugImGuiFeature_NetworkingGatherDataInput(const UWorld& _World, UObject* _OwnerObject, FKLDebugImGuiFeatureContext_Base* _ContextData, FArchive& _Archive);

    UE_NODISCARD const UWorld& GetWorld() const;

    // the owner is guaranteed to be valid only for IKLDebugImGuiFeatureInterface_Selectable
    template <typename CallingClass>
    UE_NODISCARD const UObject& TryGetOwnerObject() const;
    template <typename CallingClass>
    UE_NODISCARD UObject& TryGetOwnerObjectMutable() const;
    template <typename CallingClass>
    UE_NODISCARD bool HasAuthorityOnObject() const;
    // the owner is guaranteed to be valid only for IKLDebugImGuiFeatureInterface_Selectable

    UE_NODISCARD FArchive& GetArchiveMutable() const;

    // the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template <typename ContextType>
    UE_NODISCARD ContextType& GetContextMutable() const;

    // the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template <typename ContextType>
    UE_NODISCARD const ContextType& GetContext() const;

private:
    UObject* mOwnerObject;
    const UWorld& mWorld;
    FKLDebugImGuiFeatureContext_Base* mContextData = nullptr;
    FArchive& mArchive;
    bool mHasAuthotityOnObject = false;
};

template <typename CallingClass>
inline UObject& FKLDebugImGuiFeature_NetworkingGatherDataInput::TryGetOwnerObjectMutable() const
{
    static_assert(TIsDerivedFrom<CallingClass, IKLDebugImGuiFeatureInterface_Selectable>::IsDerived, "Only classes derived from IKLDebugImGuiFeatureInterface_Selectable can call this method");

    check(mOwnerObject != nullptr);
    return *mOwnerObject;
}

template <typename CallingClass>
inline const UObject& FKLDebugImGuiFeature_NetworkingGatherDataInput::TryGetOwnerObject() const
{
    static_assert(TIsDerivedFrom<CallingClass, IKLDebugImGuiFeatureInterface_Selectable>::IsDerived, "Only classes derived from IKLDebugImGuiFeatureInterface_Selectable can call this method");

    return TryGetOwnerObjectMutable<CallingClass>();
}

template <typename CallingClass>
inline bool FKLDebugImGuiFeature_NetworkingGatherDataInput::HasAuthorityOnObject() const
{
    static_assert(TIsDerivedFrom<CallingClass, IKLDebugImGuiFeatureInterface_Selectable>::IsDerived, "Only classes derived from IKLDebugImGuiFeatureInterface_Selectable can call this method");

    return mHasAuthotityOnObject;
}

inline const UWorld& FKLDebugImGuiFeature_NetworkingGatherDataInput::GetWorld() const
{
    return mWorld;
}

inline FArchive& FKLDebugImGuiFeature_NetworkingGatherDataInput::GetArchiveMutable() const
{
    return mArchive;
}

template <typename ContextType>
inline ContextType& FKLDebugImGuiFeature_NetworkingGatherDataInput::GetContextMutable() const
{
    checkf(mContextData && mContextData->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContextData);
}

template <typename ContextType>
inline const ContextType& FKLDebugImGuiFeature_NetworkingGatherDataInput::GetContext() const
{
    return GetContextMutable<ContextType>();
}