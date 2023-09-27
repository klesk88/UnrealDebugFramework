#include "Features/AI/KLDebugFeatureAI_BehaviorTree.h"

#include "Features/AI/KLDebugFeatureAI_BehaviorTreeNetworkContext.h"
#include "Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"
#include "Filters/KLDebugImGuiFilterAI.h"

// imgui module
#include "ImGui/Public/Helpers/KLDebugImGuiHelpers.h"
//imgui networking module
#include "ImGuiNetworking/Runtime/Public/Interface/Input/KLDebugImGuiNetworking_GatherDataInput.h"
#include "ImGuiNetworking/Runtime/Public/Interface/Input/KLDebugImGuiNetworking_ReceiveDataInput.h"
// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"

KL_DEBUG_CREATE_WINDOW(FKLDebugFeatureAI_BehaviorTree)

TUniquePtr<FKLDebugImGuiFeatureContext_Base> FKLDebugFeatureAI_BehaviorTree::GetFeatureContext(const FKLDebugImGuiFeatureContextInput& _Input) const
{
    switch (_Input.GetCurrentNetMode())
    {
    case ENetMode::NM_DedicatedServer:
    case ENetMode::NM_Client:
        return MakeUnique<FKLDebugFeatureAI_BehaviorTreeNetworkContext>();
    case ENetMode::NM_Standalone:
    case ENetMode::NM_ListenServer:
    case ENetMode::NM_MAX:
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

void FKLDebugFeatureAI_BehaviorTree::DrawImGuiChild(const FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable& _Input)
{
    switch (_Input.GetWorld().GetNetMode())
    {
    case ENetMode::NM_Standalone:
    case ENetMode::NM_ListenServer:
    case ENetMode::NM_MAX:
    {
        FKLDebugFeatureAI_BehaviorTreeNetworkContext Context;
        Context.UpdateData(_Input.GetObject());
        ImGuiDrawBrainInfo(Context);
        ImGuiDrawBTInfo(Context);
    }
        break;
    case ENetMode::NM_DedicatedServer:
        checkNoEntry();
        break;
    case ENetMode::NM_Client:
    {
        const FKLDebugFeatureAI_BehaviorTreeNetworkContext& Context = _Input.GetContext<FKLDebugFeatureAI_BehaviorTreeNetworkContext>();
        ImGuiDrawBrainInfo(Context);
        ImGuiDrawBTInfo(Context);
    }
        break;
    }
}

void FKLDebugFeatureAI_BehaviorTree::ImGuiDrawBrainInfo(const FKLDebugFeatureAI_BehaviorTreeNetworkContext& _Context) const
{
    if (_Context.GetBrainComponentDebugInfo().IsEmpty())
    {
        ImGui::Text("No Brain Component");
        return;
    }

    FString TempString;
    TempString.Reserve(300);

    if (ImGui::TreeNode("BehaviorData"))
    {
        ImGui::Text("BrainComponent: [%ls]", *_Context.GetBrainComponentDebugInfo());
        ImGui::TreePop();
    }

    for (int32 Idx = 0; Idx < _Context.GetBlackboardLines().Num(); Idx++)
    {
        int32 SeparatorIndex = INDEX_NONE;
        _Context.GetBlackboardLines()[Idx].FindChar(TEXT(':'), SeparatorIndex);

        if (SeparatorIndex != INDEX_NONE && Idx)
        {
            TempString = _Context.GetBlackboardLines()[Idx].Left(SeparatorIndex + 1);
            ImGui::Text("%ls", *TempString);
            ImGui::SameLine();
            TempString = _Context.GetBlackboardLines()[Idx].Mid(SeparatorIndex + 1);
            ImGui::TextColored(KL::Debug::ImGuiHelpers::ConvertUnrealColorToImGui(FColor::Yellow), TCHAR_TO_ANSI(*TempString));
        }
        else
        {
            ImGui::Text("%ls", *_Context.GetBlackboardLines()[Idx]);
        }
    }

    ImGui::TreePop();
}

void FKLDebugFeatureAI_BehaviorTree::ImGuiDrawBTInfo(const FKLDebugFeatureAI_BehaviorTreeNetworkContext& _Context) const
{
    if (!_Context.GetCurrentAIAssets().IsEmpty())
    {
        return;
    }

    ImGui::Text("Behavior:");
    ImGui::SameLine();
    KL::Debug::ImGuiHelpers::DrawColoredText(FColor::Yellow, _Context.GetCurrentAIState());
    ImGui::SameLine();
    ImGui::Text("Tree:");
    ImGui::SameLine();
    KL::Debug::ImGuiHelpers::DrawColoredText(FColor::Yellow, _Context.GetCurrentAIAssets());

    ImGui::Text("Active task:");
    ImGui::SameLine();
    KL::Debug::ImGuiHelpers::DrawColoredText(FColor::Yellow, _Context.GetCurrentAITask());
}

bool FKLDebugFeatureAI_BehaviorTree::ShouldGatherData(const FKLDebugImGuiNetworking_GatherDataInput& _GatherDataInput) const
{
    const FKLDebugFeatureAI_BehaviorTreeNetworkContext& Context = _GatherDataInput.GetContext<FKLDebugFeatureAI_BehaviorTreeNetworkContext>();
    return Context.ShouldGatherData(_GatherDataInput);
}

void FKLDebugFeatureAI_BehaviorTree::GatherData(const FKLDebugImGuiNetworking_GatherDataInput& _GatherDataInput) const
{
    switch (_GatherDataInput.GetCurrentEnviroment())
    {
    case EKLDebugImGuiNetworkingEnviroment::Server:
    {
        FKLDebugFeatureAI_BehaviorTreeNetworkContext& Context = _GatherDataInput.GetContextMutable<FKLDebugFeatureAI_BehaviorTreeNetworkContext>();
        Context.UpdateData(_GatherDataInput.GetOwnerObject());
        Context.Network_GatherData(_GatherDataInput.GetArchiveMutable());
    }

        break;
    case EKLDebugImGuiNetworkingEnviroment::Client:
        break;
    }
}

void FKLDebugFeatureAI_BehaviorTree::ReceiveData(const FKLDebugImGuiNetworking_ReceiveDataInput& _Input)
{
    FKLDebugFeatureAI_BehaviorTreeNetworkContext& Context = _Input.GetContextMutable<FKLDebugFeatureAI_BehaviorTreeNetworkContext>();
    Context.Network_GatherData(_Input.GetArchiveMutable());
}
