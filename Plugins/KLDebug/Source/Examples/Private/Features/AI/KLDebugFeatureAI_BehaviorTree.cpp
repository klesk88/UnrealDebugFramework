// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/AI/KLDebugFeatureAI_BehaviorTree.h"

#include "Features/AI/KLDebugFeatureAI_BehaviorTreeNetworkContext.h"
#include "Filters/KLDebugImGuiFilterAI.h"

// modules
#include "User/Framework/Public/Feature/Interface/Selectable/KLDebugFeatureSelectableAllInputs.h"
#include "User/Framework/Public/Networking/Feature/Selectable/KLDebugUserNetworkingFeatureSelectableAllInputs.h"
#include "User/Framework/Public/ThirdParty/ImGui/Helpers/KLDebugImGuiHelpers.h"
#include "User/ThirdParty/ImGui/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"

KL_DEBUG_CREATE_FEATURE(FKLDebugFeatureAI_BehaviorTree)

TUniquePtr<IKLDebugContextInterface> FKLDebugFeatureAI_BehaviorTree::GetContext(const FKLDebugContextGetterInput_Selectable& _Input) const
{
    switch (_Input.GetCurrentNetMode())
    {
    case ENetMode::NM_DedicatedServer:
        if (_Input.HasAuthorityOnObject())
        {
            return MakeUnique<FKLDebugFeatureAI_BehaviorTreeNetworkContext>();
        }
        return nullptr;
    case ENetMode::NM_Client:
        if (!_Input.HasAuthorityOnObject())
        {
            return MakeUnique<FKLDebugFeatureAI_BehaviorTreeNetworkContext>();
        }
        return nullptr;
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

void FKLDebugFeatureAI_BehaviorTree::DrawImGuiChild(FKLDebugFeatureImGuiInput_Selectable& _Input)
{
    if (_Input.HasAuthorityOnObject())
    {
        FKLDebugFeatureAI_BehaviorTreeNetworkContext Context;
        Context.UpdateData(_Input.GetObject());
        ImGuiDrawBrainInfo(Context);
        ImGuiDrawBTInfo(Context);
    }
    else
    {
        const FKLDebugFeatureAI_BehaviorTreeNetworkContext& Context = _Input.GetContext<FKLDebugFeatureAI_BehaviorTreeNetworkContext>();
        ImGuiDrawBrainInfo(Context);
        ImGuiDrawBTInfo(Context);
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
        ImGui::Text("BrainComponent: [%s]", TCHAR_TO_ANSI(*_Context.GetBrainComponentDebugInfo()));
        ImGui::TreePop();
    }

    for (int32 Idx = 0; Idx < _Context.GetBlackboardLines().Num(); Idx++)
    {
        int32 SeparatorIndex = INDEX_NONE;
        _Context.GetBlackboardLines()[Idx].FindChar(TEXT(':'), SeparatorIndex);

        if (SeparatorIndex != INDEX_NONE && Idx)
        {
            TempString = _Context.GetBlackboardLines()[Idx].Left(SeparatorIndex + 1);
            ImGui::Text("%s", TCHAR_TO_ANSI(*TempString));
            ImGui::SameLine();
            TempString = _Context.GetBlackboardLines()[Idx].Mid(SeparatorIndex + 1);
            ImGui::TextColored(KL::Debug::ImGuiHelpers::ConvertUnrealColorToImGui(FColor::Yellow), "%s", TCHAR_TO_ANSI(*TempString));
        }
        else
        {
            ImGui::Text("%s", TCHAR_TO_ANSI(*_Context.GetBlackboardLines()[Idx]));
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

bool FKLDebugFeatureAI_BehaviorTree::Server_ShouldTick(const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input) const
{
    const FKLDebugFeatureAI_BehaviorTreeNetworkContext& Context = _Input.GetContext<FKLDebugFeatureAI_BehaviorTreeNetworkContext>();
    return Context.ShouldGatherData(_Input);
}

void FKLDebugFeatureAI_BehaviorTree::Server_Tick(const FKLDebugUserNetworkingFeatureSelectableServerTickInput& _Input)
{
    FKLDebugFeatureAI_BehaviorTreeNetworkContext& Context = _Input.GetContextMutable<FKLDebugFeatureAI_BehaviorTreeNetworkContext>();
    Context.UpdateData(_Input.GetObject());
    Context.Network_GatherData(_Input.GetArchiveMutable());
}

void FKLDebugFeatureAI_BehaviorTree::Client_ReceiveData(const FKLDebugUserNetworkingFeatureSelectableReceiveDataInput& _Input)
{
    FKLDebugFeatureAI_BehaviorTreeNetworkContext& Context = _Input.GetContextMutable<FKLDebugFeatureAI_BehaviorTreeNetworkContext>();
    Context.Network_GatherData(_Input.GetArchiveMutable());
}
