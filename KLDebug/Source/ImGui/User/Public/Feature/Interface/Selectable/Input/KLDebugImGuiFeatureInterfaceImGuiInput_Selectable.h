#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "Feature/Interface/Input/KLDebugImGuiFeatureInterfaceImGuiInput_Base.h"

class UObject;
class UWorld;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable final : public FKLDebugImGuiFeatureInterfaceImGuiInput_Base
{
public:
    explicit FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable(const UWorld& _World, bool& _IsWindowOpen, FKLDebugImGuiFeatureContext_Base* _FeatureContext, UObject& _Object);

    UE_NODISCARD const UObject& GetObject() const;
    UE_NODISCARD UObject& GetObjectMutable() const;

    //the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    //It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template<typename ContextType>
    UE_NODISCARD ContextType& GetContextMutable() const;

    //the user is expected to know the type of the context the imgui spawns trough GetFeatureContext.
    //It is also responsible to know when is valid and when not (if spawn is guaranteed to be valid till the imgui feature is valid)
    template<typename ContextType>
    UE_NODISCARD const ContextType& GetContext() const;

private:
    UObject& mObject;
    FKLDebugImGuiFeatureContext_Base* mContextData = nullptr;
};

inline const UObject& FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable::GetObject() const
{
    return GetObjectMutable();
}

inline UObject& FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable::GetObjectMutable() const
{
    return mObject;
}

template<typename ContextType>
inline ContextType& FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable::GetContextMutable() const
{
    checkf(mContextData && mContextData->IsDerivedFrom<ContextType>(), TEXT("casting to wrong type or calling to a nullptr. User is expected to call this method only when a context is valid on the right type"));

    return *static_cast<ContextType*>(mContextData);
}

template<typename ContextType>
inline const ContextType& FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable::GetContext() const
{
    return GetContextMutable<ContextType>();
}