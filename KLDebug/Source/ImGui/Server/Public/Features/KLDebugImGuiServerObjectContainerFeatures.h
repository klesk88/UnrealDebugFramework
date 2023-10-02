#pragma once

#include "Features/KLDebugImGuiServerObjectFeatureData.h"

//modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeatureContextInput;
class IKLDebugImGuiFeatureInterfaceBase;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerObjectContainerFeatures final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiServerObjectContainerFeatures(const EImGuiInterfaceType _ContainerType);

    void InitIfNeeded();

    void AddFeature(const FKLDebugImGuiFeatureContextInput& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex);
    void RemoveFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex);

    UE_NODISCARD const TArray<FKLDebugImGuiServerObjectFeatureData>& GetEnableFetures() const;
    UE_NODISCARD EImGuiInterfaceType GetContainerType() const;

private:
    TArray<FKLDebugImGuiServerObjectFeatureData> mFeaturesEnable;
    EImGuiInterfaceType mContainerType = EImGuiInterfaceType::COUNT;
};

inline void FKLDebugImGuiServerObjectContainerFeatures::AddFeature(const FKLDebugImGuiFeatureContextInput& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
{
    ensureMsgf(mFeaturesEnable.IndexOfByKey(_FeatureIndex) == INDEX_NONE, TEXT("Feature alreadu present"));

    mFeaturesEnable.Emplace(_Input, _FeatureInterface, _FeatureIndex);
}

inline void FKLDebugImGuiServerObjectContainerFeatures::RemoveFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
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

inline const TArray<FKLDebugImGuiServerObjectFeatureData>& FKLDebugImGuiServerObjectContainerFeatures::GetEnableFetures() const
{
    return mFeaturesEnable;
}

inline EImGuiInterfaceType FKLDebugImGuiServerObjectContainerFeatures::GetContainerType() const
{
    return mContainerType;
}