// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureReceiveDataInputBase.h"

class UObject;

/*
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingFeatureSelectableReceiveDataInput final : public FKLDebugUserNetworkingFeatureReceiveDataInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureSelectableReceiveDataInput(const EKLDebugNetworkReceiveMessageType _ReceiveType, const UWorld& _World, UObject& _Object, IKLDebugContextInterface* _FeatureContext, FArchive& _Archive);

    [[nodiscard]] UObject& GetObjectMutable() const;
    [[nodiscard]] const UObject& GetObject() const;

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
