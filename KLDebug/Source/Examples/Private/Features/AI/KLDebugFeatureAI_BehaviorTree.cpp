#include "Features/AI/KLDebugFeatureAI_BehaviorTree.h"

#include "Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"
#include "Filters/KLDebugImGuiFilterAI.h"

// imgui module
#include "ImGui/Public/Helpers/KLDebugImGuiHelpers.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "GameFramework/Pawn.h"

KL_DEBUG_CREATE_WINDOW(FKLDebugFeatureAI_BehaviorTree)

bool FKLDebugFeatureAI_BehaviorTree::DoesSupportObject(const UObject& _Object) const
{
    return GetBTComponent(_Object) != nullptr;
}

TUniquePtr<FKLDebugImGuiFeatureContext_Base> FKLDebugFeatureAI_BehaviorTree::GetFeatureContext(const FKLDebugImGuiFeatureContextInput& _Input) const
{
    switch (_Input.GetCurrentNetMode())
    {
    case ENetMode::NM_Client:
        break;
    case ENetMode::NM_DedicatedServer:
        break;
    default:
        break;
    }

    return nullptr;
}

const FString& FKLDebugFeatureAI_BehaviorTree::GetWindowName() const
{
    static const FString Name(TEXT("BehaviorTree"));
    return Name;
}

void FKLDebugFeatureAI_BehaviorTree::GetFilterPath(TArray<FName>& _OutFilters) const
{
    GetFilterPathHelper<KLDebugImGuiFilterAI>(_OutFilters);
}

const FName& FKLDebugFeatureAI_BehaviorTree::GetImGuiPath() const
{
    static const FName Path(TEXT("AI.Behavior.BehaviorTree"));
    return Path;
}

const UBehaviorTreeComponent* FKLDebugFeatureAI_BehaviorTree::GetBTComponent(const UObject& _Object) const
{
    return Cast<const UBehaviorTreeComponent>(GetBrainComponent(_Object));
}

const UBrainComponent* FKLDebugFeatureAI_BehaviorTree::GetBrainComponent(const UObject& _Object) const
{
    const APawn*           MyPawn       = Cast<APawn>(&_Object);
    const AAIController*   MyController = MyPawn ? Cast<AAIController>(MyPawn->Controller) : nullptr;
    const UBrainComponent* BrainComp    = GetValid(MyController ? MyController->GetBrainComponent() : nullptr);

    return BrainComp;
}

void FKLDebugFeatureAI_BehaviorTree::DrawImGuiChild(const FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable& _Input)
{
    switch (_Input.GetWorld().GetNetMode())
    {
    case ENetMode::NM_Standalone:
    case ENetMode::NM_ListenServer:     
        ImGuiDrawBrainInfo(_Input.GetObject());
        ImGuiDrawBTInfo(_Input.GetObject()); 
        break;
    default:
        break;
    }
}

void FKLDebugFeatureAI_BehaviorTree::ImGuiDrawBrainInfo(const UObject& _Object) const
{
    // based on FGameplayDebuggerCategory_BehaviorTree::DrawData

    const UBrainComponent* BrainComponent = GetBrainComponent(_Object);
    if (!BrainComponent)
    {
        ImGui::Text("No Brain Component");
        return;
    }

    FString                TempString;
    TempString.Reserve(300);

    if (ImGui::TreeNode("BehaviorData"))
    {
        TempString = BrainComponent->GetDebugInfoString();
        ImGui::Text("BrainComponent: [%ls]", *TempString);
        ImGui::TreePop();
    }

    const UBlackboardComponent* BBComponent = BrainComponent->GetBlackboardComponent();
    if (!BBComponent || !ImGui::TreeNode("BlackboardData"))
    {
        return;
    }

    const FString   BBDescription = BBComponent->GetDebugInfoString(EBlackboardDescription::KeyWithValue);
    TArray<FString> BlackboardLines;
    BBDescription.ParseIntoArrayLines(BlackboardLines, true);

    for (int32 Idx = 0; Idx < BlackboardLines.Num(); Idx++)
    {
        int32 SeparatorIndex = INDEX_NONE;
        BlackboardLines[Idx].FindChar(TEXT(':'), SeparatorIndex);

        if (SeparatorIndex != INDEX_NONE && Idx)
        {
            TempString = BlackboardLines[Idx].Left(SeparatorIndex + 1);
            ImGui::Text("%ls", *TempString);
            ImGui::SameLine();
            TempString = BlackboardLines[Idx].Mid(SeparatorIndex + 1);
            ImGui::TextColored(KL::Debug::ImGuiHelpers::ConvertUnrealColorToImGui(FColor::Yellow), TCHAR_TO_ANSI(*TempString));
        }
        else
        {
            ImGui::Text("%ls", *BlackboardLines[Idx]);
        }
    }

    ImGui::TreePop();
}

void FKLDebugFeatureAI_BehaviorTree::ImGuiDrawBTInfo(const UObject& _Object) const
{
    // based on FGameplayDebuggerCategory_AI::DrawData

    const UBehaviorTreeComponent* BTComponent = GetBTComponent(_Object);
    if (!BTComponent)
    {
        return;
    }
    const FString CurrentAITask   = BTComponent->DescribeActiveTasks();
    const FString CurrentAIState  = BTComponent->IsRunning() ? TEXT("Running") : BTComponent->IsPaused() ? TEXT("Paused")
                                                                                                         : TEXT("Inactive");
    const FString CurrentAIAssets = BTComponent->DescribeActiveTrees();

    ImGui::Text("Behavior:");
    ImGui::SameLine();
    KL::Debug::ImGuiHelpers::DrawColoredText(FColor::Yellow, CurrentAIState);
    ImGui::SameLine();
    ImGui::Text("Tree:");
    ImGui::SameLine();
    KL::Debug::ImGuiHelpers::DrawColoredText(FColor::Yellow, CurrentAIAssets);

    ImGui::Text("Active task:");
    ImGui::SameLine();
    KL::Debug::ImGuiHelpers::DrawColoredText(FColor::Yellow, CurrentAITask);
}