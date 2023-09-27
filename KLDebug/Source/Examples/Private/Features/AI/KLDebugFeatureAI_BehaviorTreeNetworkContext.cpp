#include "Features/AI/KLDebugFeatureAI_BehaviorTreeNetworkContext.h"

//ImGuiNetworking module
#include "ImGuiNetworking/Runtime/Public/Interface/Input/KLDebugImGuiNetworking_GatherDataInput.h"

// engine
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Containers/UnrealString.h"
#include "GameFramework/Pawn.h"
#include "Serialization/Archive.h"

bool FKLDebugFeatureAI_BehaviorTreeNetworkContext::ShouldGatherData(const FKLDebugImGuiNetworking_GatherDataInput& _GatherDataInput) const
{
    const float TimeSinceReplicated = GetTimeSinceReplicated(_GatherDataInput.GetWorld());
    return TimeSinceReplicated > 0.5f;
}

void FKLDebugFeatureAI_BehaviorTreeNetworkContext::UpdateData(const UObject& _OwnerObject)
{
    UpdateBrainInfo(_OwnerObject);
    UpateBTInfo(_OwnerObject);
}

void FKLDebugFeatureAI_BehaviorTreeNetworkContext::Network_GatherData(FArchive& _Archive)
{
    _Archive << mRepBlackboardLines;
    _Archive << mRepBrainComponentDebugInfo;
    _Archive << mRepCurrentAITask;
    _Archive << mRepCurrentAIState;
    _Archive << mRepCurrentAIAssets;
}

const UBehaviorTreeComponent* FKLDebugFeatureAI_BehaviorTreeNetworkContext::GetBTComponent(const UObject& _Object) const
{
    return Cast<const UBehaviorTreeComponent>(GetBrainComponent(_Object));
}

const UBrainComponent* FKLDebugFeatureAI_BehaviorTreeNetworkContext::GetBrainComponent(const UObject& _Object) const
{
    const APawn* MyPawn = Cast<APawn>(&_Object);
    const AAIController* MyController = MyPawn ? Cast<AAIController>(MyPawn->Controller) : nullptr;
    const UBrainComponent* BrainComp = GetValid(MyController ? MyController->GetBrainComponent() : nullptr);

    return BrainComp;
}

void FKLDebugFeatureAI_BehaviorTreeNetworkContext::UpdateBrainInfo(const UObject& _Object)
{
    // based on FGameplayDebuggerCategory_AI::DrawData

    const UBrainComponent* BrainComponent = GetBrainComponent(_Object);
    if (!BrainComponent)
    {
        return;
    }

    mRepBrainComponentDebugInfo = BrainComponent->GetDebugInfoString();

    const UBlackboardComponent* BBComponent = BrainComponent->GetBlackboardComponent();
    if (!BBComponent)
    {
        return;
    }

    const FString BBDescription = BBComponent->GetDebugInfoString(EBlackboardDescription::KeyWithValue);
    BBDescription.ParseIntoArrayLines(mRepBlackboardLines, true);
}

void FKLDebugFeatureAI_BehaviorTreeNetworkContext::UpateBTInfo(const UObject& _Object)
{
    // based on FGameplayDebuggerCategory_AI::DrawData

    const UBehaviorTreeComponent* BTComponent = GetBTComponent(_Object);
    if (!BTComponent)
    {
        return;
    }

    mRepCurrentAITask = BTComponent->DescribeActiveTasks();
    mRepCurrentAIState = BTComponent->IsRunning() ? TEXT("Running") : BTComponent->IsPaused() ? TEXT("Paused") : TEXT("Inactive");
    mRepCurrentAIAssets = BTComponent->DescribeActiveTrees();
}

