// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Context/KLDebugContextInterface.h"
#include "Feature/Interface/Input/KLDebugFeatureInputFlags.h"

// engine
#include "Templates/UnrealTemplate.h"

/*
 * Our wrapper class around FGameplayDebuggerCanvasContext. This is just in case we want to add something on our side at some point
 * or if Epic decides to change FGameplayDebuggerCanvasContext then we can just update this class to contains what it needs
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureTickInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugFeatureTickInput_Base(const UWorld& _World, IKLDebugContextInterface* _Context);
    virtual ~FKLDebugFeatureTickInput_Base() = default;

    UE_NODISCARD FKLDebugFeatureInputFlags& GetUpdateFlagsMutable();
    UE_NODISCARD const FKLDebugFeatureInputFlags& GetUpdateFlags() const;

    UE_NODISCARD const UWorld& GetWorld() const;

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
    IKLDebugContextInterface* mContext = nullptr;
    FKLDebugFeatureInputFlags mUpdateFlags;
};

inline const UWorld& FKLDebugFeatureTickInput_Base::GetWorld() const
{
    return mWorld;
}

template <typename ContextType>
inline ContextType& FKLDebugFeatureTickInput_Base::GetContextMutable() const
{
    checkf(mContext && mContext->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContext);
}

template <typename ContextType>
inline const ContextType& FKLDebugFeatureTickInput_Base::GetContext() const
{
    return GetContextMutable<ContextType>();
}

inline FKLDebugFeatureInputFlags& FKLDebugFeatureTickInput_Base::GetUpdateFlagsMutable()
{
    return mUpdateFlags;
}

inline const FKLDebugFeatureInputFlags& FKLDebugFeatureTickInput_Base::GetUpdateFlags() const
{
    return mUpdateFlags;
}