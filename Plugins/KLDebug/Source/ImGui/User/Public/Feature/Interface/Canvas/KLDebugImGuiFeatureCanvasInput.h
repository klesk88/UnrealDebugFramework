// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"

// engine
#include "GameplayDebuggerTypes.h"

class UCanvas;
class UFont;

/*
 * Our wrapper class around FGameplayDebuggerCanvasContext. This is just in case we want to add something on our side at some point
 * or if Epic decides to change FGameplayDebuggerCanvasContext then we can just update this class to contains what it needs
 */
class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureCanvasInput final : public FGameplayDebuggerCanvasContext
{
public:
    explicit FKLDebugImGuiFeatureCanvasInput(UCanvas& _Canvas, UFont& _Font);

    void SetContext(FKLDebugImGuiFeatureContext_Base* _Context);
    void SetOwnerObject(const UObject& _Owner);

    // this for unique features will be the world while for the selectable one will be the actor which is selected in the editor
    // and this features is working on
    UE_NODISCARD const UObject& GetOwnerObject() const;

    // the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template <typename ContextType>
    UE_NODISCARD ContextType& GetContextMutable() const;

    // the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template <typename ContextType>
    UE_NODISCARD const ContextType& GetContext() const;

private:
    FKLDebugImGuiFeatureContext_Base* mContext = nullptr;
    const UObject* mOwnerObject = nullptr;
};

inline void FKLDebugImGuiFeatureCanvasInput::SetContext(FKLDebugImGuiFeatureContext_Base* _Context)
{
    mContext = _Context;
}

inline void FKLDebugImGuiFeatureCanvasInput::SetOwnerObject(const UObject& _Owner)
{
    mOwnerObject = &_Owner;
}

inline const UObject& FKLDebugImGuiFeatureCanvasInput::GetOwnerObject() const
{
    checkf(mOwnerObject != nullptr, TEXT("owner must be valid"));
    return *mOwnerObject;
}

template <typename ContextType>
inline ContextType& FKLDebugImGuiFeatureCanvasInput::GetContextMutable() const
{
    checkf(mContext && mContext->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContext);
}

template <typename ContextType>
inline const ContextType& FKLDebugImGuiFeatureCanvasInput::GetContext() const
{
    return GetContextMutable<ContextType>();
}
