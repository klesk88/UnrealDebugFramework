// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureClientTickInputBase.h"

class UObject;

class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingFeatureSelectableClientTickInput final : public FKLDebugUserNetworkingFeatureClientTickInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureSelectableClientTickInput(const UWorld& _World, UObject& _OwnerObject, IKLDebugContextInterface* _Context, FArchive& _Archive);

    [[nodiscard]] UObject& GetObjectMutable() const;
    [[nodiscard]] const UObject& GetObject() const;

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
