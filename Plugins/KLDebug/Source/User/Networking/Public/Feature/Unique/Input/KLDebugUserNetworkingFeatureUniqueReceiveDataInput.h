// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureReceiveDataInputBase.h"

/*
 */
class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingFeatureUniqueReceiveDataInput final : public FKLDebugUserNetworkingFeatureReceiveDataInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureUniqueReceiveDataInput(const EKLDebugNetworkReceiveMessageType _ReceiveType, const UWorld& _World, FKLDebugImGuiFeatureContext_Base* _FeatureContext, FArchive& _Archive);
};
