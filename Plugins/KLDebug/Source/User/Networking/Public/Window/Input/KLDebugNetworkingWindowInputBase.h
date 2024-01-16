// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "User/Framework/Public/Context/KLDebugContextInterface.h"

// engine
#include "Serialization/Archive.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGUSERNETWORKING_API FKLDebugNetworkingWindowInputBase : public FNoncopyable
{
public:
    explicit FKLDebugNetworkingWindowInputBase(const UWorld& _World, IKLDebugContextInterface* _Context, FArchive& _Archive);
    virtual ~FKLDebugNetworkingWindowInputBase() = default;

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD FArchive& GetArchiveMutable() const;

    // the user is expected to know the type of the context.
    template <typename ContextType>
    UE_NODISCARD ContextType& GetContextMutable() const;

    // the user is expected to know the type of the context.
    template <typename ContextType>
    UE_NODISCARD const ContextType& GetContext() const;

private:
    const UWorld& mWorld;
    FArchive& mArchive;
    IKLDebugContextInterface* mContextData = nullptr;
};

inline const UWorld& FKLDebugNetworkingWindowInputBase::GetWorld() const
{
    return mWorld;
}

inline FArchive& FKLDebugNetworkingWindowInputBase::GetArchiveMutable() const
{
    return mArchive;
}

template <typename ContextType>
inline ContextType& FKLDebugNetworkingWindowInputBase::GetContextMutable() const
{
    checkf(mContextData && mContextData->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));
    return *static_cast<ContextType*>(mContextData);
}

template <typename ContextType>
inline const ContextType& FKLDebugNetworkingWindowInputBase::GetContext() const
{
    return GetContextMutable<ContextType>();
}