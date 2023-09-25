#pragma once

//imgui module
#include "ImGui/Public/Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManagerTypes.h"
#include "ImGui/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGIMGUINETWORKINGSERVER_API FKLDebugImGuiNetworking_ServerObjectContainerFeatures final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiNetworking_ServerObjectContainerFeatures(const EContainerType _ContainerType);

    void InitIfNeeded();

    void AddFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex);
    void RemoveFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex);

private:
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> mFeaturesEnable;
    EContainerType mContainerType = EContainerType::COUNT;
};

inline void FKLDebugImGuiNetworking_ServerObjectContainerFeatures::AddFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
{
    ensureMsgf(mFeaturesEnable.Find(_FeatureIndex) == INDEX_NONE, TEXT("Feature alreadu present"));

    mFeaturesEnable.Emplace(_FeatureIndex);
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