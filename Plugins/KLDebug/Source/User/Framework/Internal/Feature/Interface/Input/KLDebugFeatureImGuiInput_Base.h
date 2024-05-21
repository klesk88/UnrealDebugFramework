// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Context/KLDebugContextInterface.h"
#include "Internal/Feature/Interface/Input/KLDebugFeatureInputFlags.h"

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureImGuiInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugFeatureImGuiInput_Base(const UWorld& _World, bool& _IsWindowOpen, IKLDebugContextInterface* _FeatureContext);
    virtual ~FKLDebugFeatureImGuiInput_Base() = default;

    [[nodiscard]] const UWorld& GetWorld() const;
    [[nodiscard]] bool& GetIsWindowOpen() const;

    [[nodiscard]] FKLDebugFeatureInputFlags& GetUpdateFlagsMutable();
    [[nodiscard]] const FKLDebugFeatureInputFlags& GetUpdateFlags() const;

    // the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template <typename ContextType>
    [[nodiscard]] ContextType& GetContextMutable() const;

    // the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template <typename ContextType>
    [[nodiscard]] const ContextType& GetContext() const;

private:
    const UWorld& mWorld;
    bool& mIsWindowOpen;
    IKLDebugContextInterface* mContextData = nullptr;
    FKLDebugFeatureInputFlags mUpdateFlags;
};

inline const UWorld& FKLDebugFeatureImGuiInput_Base::GetWorld() const
{
    return mWorld;
}

inline bool& FKLDebugFeatureImGuiInput_Base::GetIsWindowOpen() const
{
    return mIsWindowOpen;
}

template <typename ContextType>
inline ContextType& FKLDebugFeatureImGuiInput_Base::GetContextMutable() const
{
    checkf(mContextData && mContextData->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContextData);
}

template <typename ContextType>
inline const ContextType& FKLDebugFeatureImGuiInput_Base::GetContext() const
{
    return GetContextMutable<ContextType>();
}

inline FKLDebugFeatureInputFlags& FKLDebugFeatureImGuiInput_Base::GetUpdateFlagsMutable()
{
    return mUpdateFlags;
}

inline const FKLDebugFeatureInputFlags& FKLDebugFeatureImGuiInput_Base::GetUpdateFlags() const
{
    return mUpdateFlags;
}