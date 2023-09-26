#pragma once

// imgui module
#include "ImGui/Public/Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"

//engine
#include "Containers/Array.h"
#include "Containers/UnrealString.h"

class FArchive;
class FKLDebugImGuiNetworking_GatherDataInput;
class UBehaviorTreeComponent;
class UBrainComponent;
class UObject;

class KLDEBUGEXAMPLES_API FKLDebugFeatureAI_BehaviorTreeNetworkContext final : public FKLDebugImGuiFeatureContext_Base
{
    DERIVED_KL_DEBUG_FEATURE_CONTEXT_CLASS(FKLDebugFeatureAI_BehaviorTreeNetworkContext, FKLDebugImGuiFeatureContext_Base)

public:
    UE_NODISCARD bool ShouldGatherData(const FKLDebugImGuiNetworking_GatherDataInput& _GatherDataInput) const;

    void UpdateData(const UObject& _OwnerObject);

    void Network_GatherData(FArchive& _Archive);

    UE_NODISCARD const TArray<FString>& GetBlackboardLines() const;
    UE_NODISCARD const FString& GetBrainComponentDebugInfo() const;
    UE_NODISCARD const FString& GetCurrentAITask() const;
    UE_NODISCARD const FString& GetCurrentAIState() const;
    UE_NODISCARD const FString& GetCurrentAIAssets() const;

private:
    UE_NODISCARD const UBehaviorTreeComponent* GetBTComponent(const UObject& _Object) const;
    UE_NODISCARD const UBrainComponent* GetBrainComponent(const UObject& _Object) const;

    void UpdateBrainInfo(const UObject& _Object);
    void UpateBTInfo(const UObject& _Object);

private:
    //replicated
    TArray<FString> mRepBlackboardLines;
    FString mRepBrainComponentDebugInfo;
    FString mRepCurrentAITask;
    FString mRepCurrentAIState;
    FString mRepCurrentAIAssets;
    //replicated
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
