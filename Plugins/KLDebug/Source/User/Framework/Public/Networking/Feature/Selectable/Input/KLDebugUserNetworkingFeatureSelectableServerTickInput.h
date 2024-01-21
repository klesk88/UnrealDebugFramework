// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureServerTickInputBase.h"

class UObject;

/*
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingFeatureSelectableServerTickInput final : public FKLDebugUserNetworkingFeatureServerTickInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureSelectableServerTickInput(const UWorld& _World, const bool _HasAuthority, UObject& _OwnerObject, IKLDebugContextInterface* _ContextData, FArchive& _Archive);

    UE_NODISCARD bool HasAuthorityOnObject() const;
    UE_NODISCARD UObject& GetObjectMutable() const;
    UE_NODISCARD const UObject& GetObject() const;

private:
    UObject& mObject;
    bool mHasAuthotityOnObject = false;
};

inline bool FKLDebugUserNetworkingFeatureSelectableServerTickInput::HasAuthorityOnObject() const
{
    return mHasAuthotityOnObject;
}

inline UObject& FKLDebugUserNetworkingFeatureSelectableServerTickInput::GetObjectMutable() const
{
    return mObject;
}

inline const UObject& FKLDebugUserNetworkingFeatureSelectableServerTickInput::GetObject() const
{
    return mObject;
}
