// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Public/Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Math/NumericLimits.h"

class FArchive;
class FKLDebugImGuiFeature_NetworkingGatherDataInput;
class UBehaviorTreeComponent;
class UBrainComponent;
class UObject;

class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_BehaviorTreeNetworkContext final : public FKLDebugImGuiFeatureContext_Base
{
    DERIVED_KL_DEBUG_FEATURE_CONTEXT_CLASS(FKLDebugFeatureAI_BehaviorTreeNetworkContext, FKLDebugImGuiFeatureContext_Base)

public:
    UE_NODISCARD bool ShouldGatherData(const FKLDebugImGuiFeature_NetworkingGatherDataInput& _GatherDataInput) const;

    void UpdateData(const UObject& _OwnerObject);

    void Network_GatherData(FArchive& _Archive);

    UE_NODISCARD const TArray<FString>& GetBlackboardLines() const;
    UE_NODISCARD const FString& GetBrainComponentDebugInfo() const;
    UE_NODISCARD const FString& GetCurrentAITask() const;
    UE_NODISCARD const FString& GetCurrentAIState() const;
    UE_NODISCARD const FString& GetCurrentAIAssets() const;

    UE_NODISCARD float GetTimeSinceReplicated(const UWorld& _World) const;

private:
    UE_NODISCARD const UBehaviorTreeComponent* GetBTComponent(const UObject& _Object) const;
    UE_NODISCARD const UBrainComponent* GetBrainComponent(const UObject& _Object) const;

    void UpdateBrainInfo(const UObject& _Object);
    void UpateBTInfo(const UObject& _Object);

private:
    // replicated
    TArray<FString> mRepBlackboardLines;
    FString mRepBrainComponentDebugInfo;
    FString mRepCurrentAITask;
    FString mRepCurrentAIState;
    FString mRepCurrentAIAssets;
    // replicated

    float mLastTimeReplicated = -1.f;
};

inline const TArray<FString>& FKLDebugFeatureAI_BehaviorTreeNetworkContext::GetBlackboardLines() const
{
    return mRepBlackboardLines;
}

inline const FString& FKLDebugFeatureAI_BehaviorTreeNetworkContext::GetBrainComponentDebugInfo() const
{
    return mRepBrainComponentDebugInfo;
}

inline const FString& FKLDebugFeatureAI_BehaviorTreeNetworkContext::GetCurrentAITask() const
{
    return mRepCurrentAITask;
}

inline const FString& FKLDebugFeatureAI_BehaviorTreeNetworkContext::GetCurrentAIState() const
{
    return mRepCurrentAIState;
}

inline const FString& FKLDebugFeatureAI_BehaviorTreeNetworkContext::GetCurrentAIAssets() const
{
    return mRepCurrentAIAssets;
}

inline float FKLDebugFeatureAI_BehaviorTreeNetworkContext::GetTimeSinceReplicated(const UWorld& _World) const
{
    return mLastTimeReplicated == -1 ? TNumericLimits<float>::Max() : _World.TimeSince(mLastTimeReplicated);
}