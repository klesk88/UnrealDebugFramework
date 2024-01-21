// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Features/KLDebugImGuiServerObjectFeatureData.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Misc/NetworkGuid.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/Object.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FKLDebugContextGetterInput_Selectable;
class FKLDebugFeatureSceneProxyInput_Selectable;
class UWorld;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerObjectFeatures final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiServerObjectFeatures(const UWorld& _World, const FNetworkGUID& _NetworkID);
    UE_NODISCARD bool operator==(const FNetworkGUID& _NetworkID) const;

    void AddFeature(const FKLDebugContextGetterInput_Selectable& _Input, const IKLDebugFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex);
    void RemoveFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex);

    UE_NODISCARD const TArray<FKLDebugImGuiServerObjectFeatureData>& GetEnableFetures() const;
    UE_NODISCARD TArray<FKLDebugImGuiServerObjectFeatureData>& GetEnableFeturesMutable();

    UE_NODISCARD UObject* GetCachedObjectMutable() const;
    UE_NODISCARD const UObject* GetCachedObject() const;
    UE_NODISCARD const FNetworkGUID& GetNetworkID() const;

private:
    TArray<FKLDebugImGuiServerObjectFeatureData> mFeaturesEnable;
    FNetworkGUID mNetworkID;
    TWeakObjectPtr<UObject> mCachedObject;
};

inline bool FKLDebugImGuiServerObjectFeatures::operator==(const FNetworkGUID& _NetworkID) const
{
    return mNetworkID == _NetworkID;
}

inline void FKLDebugImGuiServerObjectFeatures::AddFeature(const FKLDebugContextGetterInput_Selectable& _Input, const IKLDebugFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex)
{
    ensureMsgf(mFeaturesEnable.IndexOfByKey(_ServerFeatureIndex) == INDEX_NONE, TEXT("Feature alreadu present"));

    mFeaturesEnable.Emplace(_Input, _FeatureInterface, _ClientFeatureIndex, _ServerFeatureIndex);
}

inline void FKLDebugImGuiServerObjectFeatures::RemoveFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
{
    const int32 Index = mFeaturesEnable.IndexOfByKey(_FeatureIndex);
    if (Index != INDEX_NONE)
    {
        mFeaturesEnable.RemoveAtSwap(Index, 1, false);
    }
}

inline const TArray<FKLDebugImGuiServerObjectFeatureData>& FKLDebugImGuiServerObjectFeatures::GetEnableFetures() const
{
    return mFeaturesEnable;
}

inline TArray<FKLDebugImGuiServerObjectFeatureData>& FKLDebugImGuiServerObjectFeatures::GetEnableFeturesMutable()
{
    return mFeaturesEnable;
}

inline UObject* FKLDebugImGuiServerObjectFeatures::GetCachedObjectMutable() const
{
    return mCachedObject.Get();
}

inline const UObject* FKLDebugImGuiServerObjectFeatures::GetCachedObject() const
{
    return GetCachedObjectMutable();
}

inline const FNetworkGUID& FKLDebugImGuiServerObjectFeatures::GetNetworkID() const
{
    return mNetworkID;
}