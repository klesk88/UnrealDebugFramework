// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureReceiveDataInputBase.h"

class UObject;

/*
 */
class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureSelectableReceiveDataInput final : public FKLDebugUserNetworkingFeatureReceiveDataInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureSelectableReceiveDataInput(const UWorld& _World, UObject& _Object, FKLDebugImGuiFeatureContext_Base* _FeatureContext, FArchive& _Archive);

    UE_NODISCARD UObject& GetObjectMutable() const;
    UE_NODISCARD const UObject& GetObject() const;

private:
    UObject& mObject;
};

inline UObject& FKLDebugUserNetworkingFeatureSelectableReceiveDataInput::GetObjectMutable() const
{
    return mObject;
}

inline const UObject& FKLDebugUserNetworkingFeatureSelectableReceiveDataInput::GetObject() const
{
    return mObject;
}
