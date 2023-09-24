#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureInterfaceImGuiInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureInterfaceImGuiInput_Base(const UWorld& _World, bool& _IsWindowOpen, FKLDebugImGuiFeatureContext_Base* _FeatureContext);
    virtual ~FKLDebugImGuiFeatureInterfaceImGuiInput_Base() = default;

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD bool& GetIsWindowOpen() const;

    template<typename FeatureContextType>
    UE_NODISCARD FeatureContextType* GetFeatureContextDataMutable() const;

    template<typename FeatureContextType>
    UE_NODISCARD const FeatureContextType* GetFeatureContextData() const;

private:
    const UWorld& mWorld;
    FKLDebugImGuiFeatureContext_Base* mFeatureContext = nullptr;
    bool& mIsWindowOpen;
};

inline const UWorld& FKLDebugImGuiFeatureInterfaceImGuiInput_Base::GetWorld() const
{
    return mWorld;
}

inline bool& FKLDebugImGuiFeatureInterfaceImGuiInput_Base::GetIsWindowOpen() const
{
    return mIsWindowOpen;
}

template<typename FeatureContextType>
FeatureContextType* FKLDebugImGuiFeatureInterfaceImGuiInput_Base::GetFeatureContextDataMutable() const
{
    checkf(!mFeatureContext || mFeatureContext->IsSameContext<FeatureContextType>(), TEXT("context data type requested is not correct"));
    return static_cast<FeatureContextType*>(mFeatureContext);
}

template<typename FeatureContextType>
const FeatureContextType* FKLDebugImGuiFeatureInterfaceImGuiInput_Base::GetFeatureContextData() const
{
    return GetFeatureContextDataMutable<FeatureContextType>();
}
