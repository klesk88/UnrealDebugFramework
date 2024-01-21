// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Context/KLDebugContextInterface.h"

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureRenderInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugFeatureRenderInput_Base(const UWorld& _World, IKLDebugContextInterface* _Context);
    virtual ~FKLDebugFeatureRenderInput_Base() = default;

    UE_NODISCARD const UWorld& GetWorld() const;

    // the user is expected to know the type of the context the spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the feature is valid)
    template <typename ContextType>
    UE_NODISCARD ContextType& GetContextMutable() const;

    // the user is expected to know the type of the context the spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the feature is valid)
    template <typename ContextType>
    UE_NODISCARD const ContextType& GetContext() const;

private:
    const UWorld& mWorld;
    IKLDebugContextInterface* mContextData = nullptr;
};

inline const UWorld& FKLDebugFeatureRenderInput_Base::GetWorld() const
{
    return mWorld;
}

template <typename ContextType>
inline ContextType& FKLDebugFeatureRenderInput_Base::GetContextMutable() const
{
    checkf(mContextData && mContextData->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContextData);
}

template <typename ContextType>
inline const ContextType& FKLDebugFeatureRenderInput_Base::GetContext() const
{
    return GetContextMutable<ContextType>();
}