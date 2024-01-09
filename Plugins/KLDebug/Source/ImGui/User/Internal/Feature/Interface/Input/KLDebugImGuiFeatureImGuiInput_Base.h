// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureImGuiInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureImGuiInput_Base(const UWorld& _World, bool& _IsWindowOpen, FKLDebugImGuiFeatureContext_Base* _FeatureContext);
    virtual ~FKLDebugImGuiFeatureImGuiInput_Base() = default;

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD bool& GetIsWindowOpen() const;

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
    bool& mIsWindowOpen;
    FKLDebugImGuiFeatureContext_Base* mContextData = nullptr;
};

inline const UWorld& FKLDebugImGuiFeatureImGuiInput_Base::GetWorld() const
{
    return mWorld;
}

inline bool& FKLDebugImGuiFeatureImGuiInput_Base::GetIsWindowOpen() const
{
    return mIsWindowOpen;
}

template <typename ContextType>
inline ContextType& FKLDebugImGuiFeatureImGuiInput_Base::GetContextMutable() const
{
    checkf(mContextData && mContextData->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContextData);
}

template <typename ContextType>
inline const ContextType& FKLDebugImGuiFeatureImGuiInput_Base::GetContext() const
{
    return GetContextMutable<ContextType>();
}
