#pragma once

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureInterfaceImGuiInput_Selectable.h"
#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureInterfaceRenderInput_Selectable.h"

// engine
#include "Containers/Array.h"

class KLDEBUGIMGUI_API IKLDebugImGuiFeatureInterface_SelectableObject : public IKLDebugImGuiFeatureInterfaceBase
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugImGuiFeatureInterface_SelectableObject, IKLDebugImGuiFeatureInterfaceBase)

public:
    virtual void GetFilterPath(TArray<FName>& _OutFilters) const = 0;

    // should return true if the object passed as input is supported by this feature
    UE_NODISCARD virtual bool DoesSupportObject(const UObject& _Object) const = 0;

    virtual void DrawImGui(const FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable& _Input);
    virtual void Render(const FKLDebugImGuiFeatureInterfaceRenderInput_Selectable& _Input) const;

protected:
    virtual void DrawImGuiChild(const FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable& _Input) = 0;

    template<typename... FilterType>
    void GetFilterPathHelper(TArray<FName>& _OutFilters) const;

private:
    template<typename... FilterType>
    void Dummy(FilterType&&...) const;
};

template<typename... FilterType>
inline void IKLDebugImGuiFeatureInterface_SelectableObject::GetFilterPathHelper(TArray<FName>& _OutFilters) const
{
    Dummy((_OutFilters.Emplace(FilterType::StaticGetFilterID()))...);

    const int32 Count = _OutFilters.Num();
    for (int32 i = 0; i < Count / 2; ++i)
    {
        _OutFilters.SwapMemory(i, Count - 1 - i);
    }
}

template<typename... FilterType>
inline void IKLDebugImGuiFeatureInterface_SelectableObject::Dummy(FilterType&&...) const
{
}

inline void IKLDebugImGuiFeatureInterface_SelectableObject::Render(const FKLDebugImGuiFeatureInterfaceRenderInput_Selectable& _Input) const
{
}