#pragma once

#include "Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"

// engine
#include "Containers/Array.h"

class KLDEBUGIMGUI_API IKLDebugImGuiFeatureInterface_SelectableObject : public IKLDebugImGuiFeatureInterfaceBase
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(IKLDebugImGuiFeatureInterface_SelectableObject, IKLDebugImGuiFeatureInterfaceBase)

public:
    virtual void GetFilterPath(TArray<FName>& _OutFilters) const = 0;

    // should return true if the object passed as input is supported by this feature
    UE_NODISCARD virtual bool DoesSupportObject(const UObject& _Object) const = 0;
};
