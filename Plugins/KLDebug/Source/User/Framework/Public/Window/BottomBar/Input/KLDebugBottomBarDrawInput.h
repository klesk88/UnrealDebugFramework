// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Context/KLDebugContextInterface.h"

// engine
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugBottomBarDrawInput final : public FNoncopyable
{
public:
    explicit FKLDebugBottomBarDrawInput(const UWorld& _World, IKLDebugContextInterface* _Context);

    UE_NODISCARD const UWorld& GetWorld() const;

    // the user is expected to know the type of the context. this is guaranteed to be valid if the user spawns one
    template <typename ContextType>
    UE_NODISCARD ContextType& GetContextMutable() const;

    // the user is expected to know the type of the context. this is guaranteed to be valid if the user spawns one
    template <typename ContextType>
    UE_NODISCARD const ContextType& GetContext() const;

private:
    const UWorld& mWorld;
    IKLDebugContextInterface* mContext = nullptr;
};

inline const UWorld& FKLDebugBottomBarDrawInput::GetWorld() const
{
    return mWorld;
}

template <typename ContextType>
inline ContextType& FKLDebugBottomBarDrawInput::GetContextMutable() const
{
    checkf(mContext && mContext->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContext);
}

template <typename ContextType>
inline const ContextType& FKLDebugBottomBarDrawInput::GetContext() const
{
    return GetContextMutable<ContextType>();
}
