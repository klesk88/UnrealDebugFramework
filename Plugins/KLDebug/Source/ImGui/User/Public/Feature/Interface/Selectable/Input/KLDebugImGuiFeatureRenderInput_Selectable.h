// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "Feature/Interface/Input/KLDebugImGuiFeatureRenderInput_Base.h"

class UWorld;
class UObject;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureRenderInput_Selectable final : public FKLDebugImGuiFeatureRenderInput_Base
{
public:
    explicit FKLDebugImGuiFeatureRenderInput_Selectable(const UWorld& _World, const UObject& _Object, FKLDebugImGuiFeatureContext_Base* _Context);

    UE_NODISCARD const UObject& GetObject() const;

    // the user is expected to know the type of the context the spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the feature is valid)
    template <typename ContextType>
    UE_NODISCARD ContextType& GetContextMutable() const;

    // the user is expected to know the type of the context the spawns trough GetFeatureContext.
    // It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the feature is valid)
    template <typename ContextType>
    UE_NODISCARD const ContextType& GetContext() const;

private:
    const UObject& mObject;
    FKLDebugImGuiFeatureContext_Base* mContextData = nullptr;
};

inline const UObject& FKLDebugImGuiFeatureRenderInput_Selectable::GetObject() const
{
    return mObject;
}

template <typename ContextType>
inline ContextType& FKLDebugImGuiFeatureRenderInput_Selectable::GetContextMutable() const
{
    checkf(mContextData && mContextData->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContextData);
}

template <typename ContextType>
inline const ContextType& FKLDebugImGuiFeatureRenderInput_Selectable::GetContext() const
{
    return GetContextMutable<ContextType>();
}