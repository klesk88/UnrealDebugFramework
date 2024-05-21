// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Context/KLDebugContextInterface.h"

// engine
#include "GameplayDebuggerTypes.h"

class UCanvas;
class UFont;
class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugModeDrawCanvasInput final : public FGameplayDebuggerCanvasContext
{
public:
    explicit FKLDebugModeDrawCanvasInput(const UWorld& _World, UCanvas& _Canvas, UFont& _Font, IKLDebugContextInterface* _Context);

    [[nodiscard]] const UWorld& GetWorld() const;

    // the user is expected to know the type of the context. this is guaranteed to be valid if the user spawns one
    template <typename ContextType>
    [[nodiscard]] ContextType& GetContextMutable() const;

    // the user is expected to know the type of the context. this is guaranteed to be valid if the user spawns one
    template <typename ContextType>
    [[nodiscard]] const ContextType& GetContext() const;

private:
    const UWorld& mWorld;
    IKLDebugContextInterface* mContext = nullptr;
};

inline const UWorld& FKLDebugModeDrawCanvasInput::GetWorld() const
{
    return mWorld;
}

template <typename ContextType>
inline ContextType& FKLDebugModeDrawCanvasInput::GetContextMutable() const
{
    checkf(mContext && mContext->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContext);
}

template <typename ContextType>
inline const ContextType& FKLDebugModeDrawCanvasInput::GetContext() const
{
    return GetContextMutable<ContextType>();
}
