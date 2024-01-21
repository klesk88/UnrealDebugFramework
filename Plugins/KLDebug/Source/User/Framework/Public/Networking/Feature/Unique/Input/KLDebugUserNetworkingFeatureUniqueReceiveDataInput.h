// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureReceiveDataInputBase.h"

/*
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingFeatureUniqueReceiveDataInput final : public FKLDebugUserNetworkingFeatureReceiveDataInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureUniqueReceiveDataInput(const EKLDebugNetworkReceiveMessageType _ReceiveType, const UWorld& _World, IKLDebugContextInterface* _FeatureContext, FArchive& _Archive);
};
