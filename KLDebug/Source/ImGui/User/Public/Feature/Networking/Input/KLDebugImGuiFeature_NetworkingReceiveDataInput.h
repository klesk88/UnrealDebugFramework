// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"

// engine
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Serialization/Archive.h"

class UWorld;

/*
 */
class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeature_NetworkingReceiveDataInput final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeature_NetworkingReceiveDataInput(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _FeatureContext, FArchive& _Archive);

    UE_NODISCARD const UWorld& GetWorld() const;
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
    const UWorld& mWorld;
    FKLDebugImGuiFeatureContext_Base* mContextData = nullptr;
    FArchive& mArchive;
};

inline const UWorld& FKLDebugImGuiFeature_NetworkingReceiveDataInput::GetWorld() const
{
    return mWorld;
}

inline FArchive& FKLDebugImGuiFeature_NetworkingReceiveDataInput::GetArchiveMutable() const
{
    return mArchive;
}

template <typename ContextType>
inline ContextType& FKLDebugImGuiFeature_NetworkingReceiveDataInput::GetContextMutable() const
{
    checkf(mContextData && mContextData->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContextData);
}

template <typename ContextType>
inline const ContextType& FKLDebugImGuiFeature_NetworkingReceiveDataInput::GetContext() const
{
    return GetContextMutable<ContextType>();
}