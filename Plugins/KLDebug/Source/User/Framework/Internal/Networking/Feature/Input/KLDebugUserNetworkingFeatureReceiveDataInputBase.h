// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Feature/Input/KLDebugUserNetworkingFeatureCommonInputBase.h"
#include "Networking/Message/KLDebugNetworkingMessageTypes.h"

// engine
#include "Serialization/Archive.h"
#include "Templates/UnrealTemplate.h"

/*
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingFeatureReceiveDataInputBase : public FKLDebugUserNetworkingFeatureCommonInputBase
{
public:
    explicit FKLDebugUserNetworkingFeatureReceiveDataInputBase(const EKLDebugNetworkReceiveMessageType _ReceiveType, const UWorld& _World, IKLDebugContextInterface* _FeatureContext, FArchive& _Archive);

    [[nodiscard]] FArchive& GetArchiveMutable() const;
    [[nodiscard]] EKLDebugNetworkReceiveMessageType GetReceiveType() const;

private:
    FArchive& mArchive;
    EKLDebugNetworkReceiveMessageType mReceiveType = EKLDebugNetworkReceiveMessageType::RequestUpdate;
};

inline FArchive& FKLDebugUserNetworkingFeatureReceiveDataInputBase::GetArchiveMutable() const
{
    return mArchive;
}

inline EKLDebugNetworkReceiveMessageType FKLDebugUserNetworkingFeatureReceiveDataInputBase::GetReceiveType() const
{
    return mReceiveType;
}