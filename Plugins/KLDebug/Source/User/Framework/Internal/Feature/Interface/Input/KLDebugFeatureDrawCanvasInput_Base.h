// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Context/KLDebugContextInterface.h"

// engine
#include "GameplayDebuggerTypes.h"

class UCanvas;
class UFont;

/*
 * Our wrapper class around FGameplayDebuggerCanvasContext. This is just in case we want to add something on our side at some point
 * or if Epic decides to change FGameplayDebuggerCanvasContext then we can just update this class to contains what it needs
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureDrawCanvasInput_Base : public FGameplayDebuggerCanvasContext
{
public:
    explicit FKLDebugFeatureDrawCanvasInput_Base(UCanvas& _Canvas, UFont& _Font, IKLDebugContextInterface* _Context);
    virtual ~FKLDebugFeatureDrawCanvasInput_Base() = default;

    // the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template <typename ContextType>
    UE_NODISCARD ContextType& GetContextMutable() const;

    // the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template <typename ContextType>
    UE_NODISCARD const ContextType& GetContext() const;

private:
    IKLDebugContextInterface* mContext = nullptr;
};

template <typename ContextType>
inline ContextType& FKLDebugFeatureDrawCanvasInput_Base::GetContextMutable() const
{
    checkf(mContext && mContext->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContext);
}

template <typename ContextType>
inline const ContextType& FKLDebugFeatureDrawCanvasInput_Base::GetContext() const
{
    return GetContextMutable<ContextType>();
}
