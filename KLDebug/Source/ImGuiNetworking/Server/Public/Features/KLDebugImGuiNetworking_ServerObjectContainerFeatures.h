#pragma once

#include "Features/KLDebugImGuiNetworking_ServerObjectFeatureData.h"

//imgui module
#include "ImGui/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeatureContextInput;
class IKLDebugImGuiFeatureInterfaceBase;

class KLDEBUGIMGUINETWORKINGSERVER_API FKLDebugImGuiNetworking_ServerObjectContainerFeatures final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiNetworking_ServerObjectContainerFeatures(const EContainerType _ContainerType);

    void InitIfNeeded();

    void AddFeature(const FKLDebugImGuiFeatureContextInput& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex);
    void RemoveFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex);

    UE_NODISCARD const TArray<FKLDebugImGuiNetworking_ServerObjectFeatureData>& GetEnableFetures() const;
    UE_NODISCARD EContainerType GetContainerType() const;

private:
    TArray<FKLDebugImGuiNetworking_ServerObjectFeatureData> mFeaturesEnable;
    EContainerType mContainerType = EContainerType::COUNT;
};

inline void FKLDebugImGuiNetworking_ServerObjectContainerFeatures::AddFeature(const FKLDebugImGuiFeatureContextInput& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
{
    ensureMsgf(mFeaturesEnable.IndexOfByKey(_FeatureIndex) == INDEX_NONE, TEXT("Feature alreadu present"));

    mFeaturesEnable.Emplace(_Input, _FeatureInterface, _FeatureIndex);
}

inline void FKLDebugImGuiNetworking_ServerObjectContainerFeatures::RemoveFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
{
    const int32 Index = mFeaturesEnable.IndexOfByKey(_FeatureIndex);
    if (Index != INDEX_NONE)
    {
        mFeaturesEnable.RemoveAtSwap(Index, 1, false);
    }
    else
    {
        ensureMsgf(false, TEXT("Feature not present"));
    }
}

inline const TArray<FKLDebugImGuiNetworking_ServerObjectFeatureData>& FKLDebugImGuiNetworking_ServerObjectContainerFeatures::GetEnableFetures() const
{
    return mFeaturesEnable;
}

inline EContainerType FKLDebugImGuiNetworking_ServerObjectContainerFeatures::GetContainerType() const
{
    return mContainerType;
}