// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureImGuiInput_Selectable.h"
#include "Feature/Interface/Selectable/Input/KLDebugImGuiFeatureRenderInput_Selectable.h"

// engine
#include "Containers/Array.h"

/*
 * This class can be used for represent data which can be selected in some form. For example this can be represented by an AActor
 * that is picked in the game viewport.
 *
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_WINDOW() in the .cpp so that the class
 * can auto subscribe to the system. For example:
 *
 * class FClassDebug final : public IKLDebugImGuiFeatureInterface_Selectable
 * {
 *      DERIVED_KL_DEBUG_FEATURE_CLASS(FClassDebug, IKLDebugImGuiFeatureInterface_Selectable)
 *      ...
 * };
 *
 * FClassDebug.cpp
 *
 * KL_DEBUG_CREATE_WINDOW(FClassDebug)
 */
class KLDEBUGIMGUIUSER_API IKLDebugImGuiFeatureInterface_Selectable : public IKLDebugImGuiFeatureInterfaceBase
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugImGuiFeatureInterface_Selectable, IKLDebugImGuiFeatureInterfaceBase)

public:
    virtual void GetFilterPath(TArray<FName>& _OutFilters) const = 0;

    virtual void DrawImGui(const FKLDebugImGuiFeatureImGuiInput_Selectable& _Input);
    virtual void Render(const FKLDebugImGuiFeatureRenderInput_Selectable& _Input) const;

    static UE_NODISCARD EImGuiInterfaceType GetInterfaceType();

protected:
    virtual void DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Selectable& _Input) = 0;

    template <typename... FilterType>
    void GetFilterPathHelper(TArray<FName>& _OutFilters) const;

private:
    template <typename... FilterType>
    void Dummy(FilterType&&...) const;
};

template <typename... FilterType>
inline void IKLDebugImGuiFeatureInterface_Selectable::GetFilterPathHelper(TArray<FName>& _OutFilters) const
{
    Dummy((_OutFilters.Emplace(FilterType::StaticGetFilterID()))...);

    const int32 Count = _OutFilters.Num();
    for (int32 i = 0; i < Count / 2; ++i)
    {
        _OutFilters.SwapMemory(i, Count - 1 - i);
    }
}

template <typename... FilterType>
inline void IKLDebugImGuiFeatureInterface_Selectable::Dummy(FilterType&&...) const
{
}

inline void IKLDebugImGuiFeatureInterface_Selectable::Render(const FKLDebugImGuiFeatureRenderInput_Selectable& _Input) const
{
}

inline EImGuiInterfaceType IKLDebugImGuiFeatureInterface_Selectable::GetInterfaceType()
{
    return EImGuiInterfaceType::SELECTABLE;
}