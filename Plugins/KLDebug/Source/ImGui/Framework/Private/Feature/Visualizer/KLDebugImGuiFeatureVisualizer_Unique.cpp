// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Unique.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"

// modules
#include "ImGui/User/Public/Feature/Interface/Unique/Input/KLDebugImGuiFeatureImGuiInput_Unique.h"
#include "ImGui/User/Public/Feature/Interface/Unique/Input/KLDebugImGuiFeatureRenderInput_Unique.h"
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"

void FKLDebugImGuiFeatureVisualizer_Unique::Init(const FKLDebugImGuiFeatureContainerBase& _Container, const FString& _TreeName, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes)
{
    FKLDebugImGuiFeatureVisualizerBase::Init(_Container, MoveTemp(_FeaturesIndexes));

    mTreeName = _TreeName;
}

void FKLDebugImGuiFeatureVisualizer_Unique::DrawImGuiTree(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context)
{
    if (ImGui::TreeNode(this, TCHAR_TO_ANSI(*mTreeName)))
    {
        const FKLDebugImGuiFeatureContextInput ContextInput{ _Context.GetCurrentNetMode(), _Context.GetWorld() };
        mTreeVisualizer.DrawImGuiTree(GetInterfaceType(), ContextInput, _Context, nullptr, mSelectedFeaturesIndexes);

        ImGui::TreePop();
    }
}

void FKLDebugImGuiFeatureVisualizer_Unique::DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context, bool& _RequireCanvasDrawing)
{
    auto Callback = [&_Context, &_RequireCanvasDrawing](FKLDebugImGuiFeatureVisualizerIterator& Iterator, FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> bool {
        IKLDebugImGuiFeatureInterface_Unique& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Unique>();
        _RequireCanvasDrawing |= Interface.RequireCanvasUpdate();
        const FKLDebugImGuiFeatureImGuiInput_Unique FeatureContext{ _Context.GetWorld(), _Entry.GetIsEnableRef() };
        Interface.DrawImGui(FeatureContext);
        return _Entry.IsEnable();
    };

    DrawImguiFeaturesEnabledCommon(_Context, Callback, nullptr);
}

void FKLDebugImGuiFeatureVisualizer_Unique::Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const
{
    const FKLDebugImGuiFeatureRenderInput_Unique FeatureContext{ _Context.GetWorld() };
    const FKLDebugImGuiFeatureContainerBase& FeaturesContainer = _Context.GetFeaturesContainerManager().GetContainer(GetInterfaceType());
    FKLDebugImGuiFeatureVisualizerConstIterator Iterator = FeaturesContainer.GetFeatureVisualizerConstIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        const IKLDebugImGuiFeatureInterface_Unique& Interface = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_Unique>();
        Interface.Render(FeatureContext);
    }
}