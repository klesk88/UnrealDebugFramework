#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerSubsystem.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Interface/Context/KLDebugImGuiFeatureContextInput.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"

//modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterface_Subsystem.h"
#include "ImGui/User/Public/Feature/Interface/Subsystem/Input/KLDebugImGuiFeatureInterfaceImGuiInput_Subsystem.h"
#include "ImGui/User/Public/Feature/Interface/Subsystem/Input/KLDebugImGuiFeatureInterfaceRenderInput_Subsystem.h"
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"

FKLDebugImGuiFeatureVisualizerSubsystem::FKLDebugImGuiFeatureVisualizerSubsystem(const FKLDebugImGuiFeatureContainerBase& _Container, const FString& _TreeName, const EContainerType _ContainerType, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes)
    : FKLDebugImGuiFeatureVisualizerBase(_Container, MoveTemp(_FeaturesIndexes))
    , mTreeName(_TreeName)
    , mContainerType(_ContainerType)
{
    checkf(mContainerType != EContainerType::COUNT, TEXT("must be initialized correctly"));
}

void FKLDebugImGuiFeatureVisualizerSubsystem::DrawImGuiTree(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context)
{
    if (ImGui::TreeNode(this, TCHAR_TO_ANSI(*mTreeName)))
    {
        const FKLDebugImGuiFeatureContextInput ContextInput{ _Context.GetCurrentNetMode(), _Context.GetWorld() };
        mTreeVisualizer.DrawImGuiTree(mContainerType, ContextInput, _Context, mSelectedFeaturesIndexes);

        ImGui::TreePop();
    }
}

void FKLDebugImGuiFeatureVisualizerSubsystem::DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context)
{
    auto Callback = [&_Context](FKLDebugImGuiFeatureVisualizerIterator& Iterator, FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> bool {
        IKLDebugImGuiFeatureInterface_Subsystem& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Subsystem>();
        const FKLDebugImGuiFeatureInterfaceImGuiInput_Subsystem FeatureContext{ _Context.GetWorld(), _Entry.GetIsEnableRef(), _Entry.TryGetFeatureContextMutable() };
        Interface.DrawImGui(FeatureContext);
        return _Entry.IsEnable();
    };

    FKLDebugImGuiFeatureContainerBase& FeaturesContainer = _Context.GetFeaturesContainerManager().GetContainerMutable(mContainerType);
    DrawImguiFeaturesEnabledCommon(_Context, mContainerType, _Context.GetWorld(), Callback);
}

void FKLDebugImGuiFeatureVisualizerSubsystem::Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const
{
    const FKLDebugImGuiFeatureInterfaceRenderInput_Subsystem FeatureContext{ _Context.GetWorld() };
    const FKLDebugImGuiFeatureContainerBase& FeaturesContainer = _Context.GetFeaturesContainerManager().GetContainer(mContainerType);
    FKLDebugImGuiFeatureVisualizerConstIterator Iterator = FeaturesContainer.GetFeatureVisualizerConstIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        const IKLDebugImGuiFeatureInterface_Subsystem& Interface = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_Subsystem>();
        Interface.Render(FeatureContext);
    }
}