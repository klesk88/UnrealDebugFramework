// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Public/Context/KLDebugContextInterface.h"

// engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Math/NumericLimits.h"

class FArchive;
class FKLDebugUserNetworkingFeatureSelectableServerTickInput;
class UBehaviorTreeComponent;
class UBrainComponent;
class UObject;

class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_BehaviorTreeNetworkContext final : public IKLDebugContextInterface
{
    KL_DEBUG_DERIVED_CONTEXT(FKLDebugFeatureAI_BehaviorTreeNetworkContext, IKLDebugContextInterface)
    
public:
    [[nodiscard]] bool ShouldGatherData(const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input) const;

    void UpdateData(const UObject& _OwnerObject);

    void Network_GatherData(FArchive& _Archive);

    [[nodiscard]] const TArray<FString>& GetBlackboardLines() const;
    [[nodiscard]] const FString& GetBrainComponentDebugInfo() const;
    [[nodiscard]] const FString& GetCurrentAITask() const;
    [[nodiscard]] const FString& GetCurrentAIState() const;
    [[nodiscard]] const FString& GetCurrentAIAssets() const;

    [[nodiscard]] float GetTimeSinceReplicated(const UWorld& _World) const;

private:
    [[nodiscard]] const UBehaviorTreeComponent* GetBTComponent(const UObject& _Object) const;
    [[nodiscard]] const UBrainComponent* GetBrainComponent(const UObject& _Object) const;

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