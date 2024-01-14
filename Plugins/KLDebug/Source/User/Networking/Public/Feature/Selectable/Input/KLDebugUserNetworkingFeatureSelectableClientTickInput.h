// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureClientTickInputBase.h"

class UObject;

class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureSelectableClientTickInput final : public FKLDebugUserNetworkingFeatureClientTickInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureSelectableClientTickInput(const UWorld& _World, UObject& _OwnerObject, FKLDebugImGuiFeatureContext_Base* _Context, FArchive& _Archive);

    UE_NODISCARD UObject& GetObjectMutable() const;
    UE_NODISCARD const UObject& GetObject() const;

private:
    UObject& mObject;
};

inline UObject& FKLDebugUserNetworkingFeatureSelectableClientTickInput::GetObjectMutable() const
{
    return mObject;
}

inline const UObject& FKLDebugUserNetworkingFeatureSelectableClientTickInput::GetObject() const
{
    return mObject;
}
