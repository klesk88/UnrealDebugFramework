// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "ImGui/User/Public/Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"

// engine
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureCommonInputBase : public FNoncopyable
{
public:
    explicit FKLDebugUserNetworkingFeatureCommonInputBase(const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _Context);
    virtual ~FKLDebugUserNetworkingFeatureCommonInputBase() = default;

    UE_NODISCARD const UWorld& GetWorld() const;

    // the user is expected to know the type of the context the feature spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the feature is valid)
    template <typename ContextType>
    UE_NODISCARD ContextType& GetContextMutable() const;

    // the user is expected to know the type of the context the feature spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the feature is valid)
    template <typename ContextType>
    UE_NODISCARD const ContextType& GetContext() const;

private:
    const UWorld& mWorld;
    FKLDebugImGuiFeatureContext_Base* mContextData = nullptr;
};

inline const UWorld& FKLDebugUserNetworkingFeatureCommonInputBase::GetWorld() const
{
    return mWorld;
}

template <typename ContextType>
inline ContextType& FKLDebugUserNetworkingFeatureCommonInputBase::GetContextMutable() const
{
    checkf(mContextData && mContextData->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));
    return *static_cast<ContextType*>(mContextData);
}

template <typename ContextType>
inline const ContextType& FKLDebugUserNetworkingFeatureCommonInputBase::GetContext() const
{
    return GetContextMutable<ContextType>();
}