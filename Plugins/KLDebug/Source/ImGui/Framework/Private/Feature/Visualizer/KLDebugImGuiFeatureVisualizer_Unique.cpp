// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizer_Unique.h"

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"

// modules
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureUniqueAllInputs.h"
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
        mTreeVisualizer.DrawImGuiTree(GetInterfaceType(), _Context, nullptr, mSelectedFeaturesIndexes);

        ImGui::TreePop();
    }
}

void FKLDebugImGuiFeatureVisualizer_Unique::DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem)
{
    auto Callback = [&_Context, &_RequiredExternalSystem](FKLDebugImGuiFeatureVisualizerIterator& _Iterator, FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> bool {
        IKLDebugImGuiFeatureInterface_Unique& Interface = _Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Unique>();
        FKLDebugImGuiFeatureImGuiInput_Unique Input{ _Context.GetWorld(), _Entry.GetIsEnableRef(), _Entry.TryGetFeatureContextMutable() };
        Interface.DrawImGui(Input);
        KL::Debug::ImGui::Features::Types::UpdateFeatureEnableSet(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy, Input.GetUpdateFlags().IsFlagSet(FKLDebugImGuiFeatureInputFlags::EFeatureUpdateFlags::SceneProxy), _RequiredExternalSystem);
        return _Entry.IsEnable();
    };

    DrawImguiFeaturesEnabledCommon(_Context, Callback, nullptr);
}

void FKLDebugImGuiFeatureVisualizer_Unique::Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const
{
    auto Callback = [&_Context](FKLDebugImGuiFeatureVisualizerConstIterator& _Iterator, const FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> void {
        FKLDebugImGuiFeatureRenderInput_Unique FeatureContext{ _Context.GetWorld(), _Entry.TryGetFeatureContextMutable() };
        const IKLDebugImGuiFeatureInterface_Unique& Interface = _Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_Unique>();
        Interface.Render(FeatureContext);
    };

    IterateOnSelectedFeatures(_Context.GetFeaturesContainerManager(), Callback);
}

void FKLDebugImGuiFeatureVisualizer_Unique::DrawOnCanvas(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, UCanvas& _Canvas, UFont& _Font, UWorld& _World) const
{
    auto Callback = [&_Canvas, &_Font, &_World](FKLDebugImGuiFeatureVisualizerConstIterator& _Iterator, const FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> void {
        const IKLDebugImGuiFeatureInterface_Unique& Interface = _Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_Unique>();
        FKLDebugFeatureDrawCanvasInput_Unique Input{ _Canvas, _Font, _Entry.TryGetFeatureContextMutable() };
        InitCommonCanvasInput(_World, Input);
        Interface.DrawOnCanvas(Input);
    };

    IterateOnSelectedFeatures(_FeatureContainerManager, Callback);
}

void FKLDebugImGuiFeatureVisualizer_Unique::GatherSceneProxies(const UPrimitiveComponent& _RenderingComponent, const KL::Debug::Framework::Rendering::GatherSceneProxyCallback& _Callback, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
{
    auto Callback = [&_Callback, &_RenderingComponent](FKLDebugImGuiFeatureVisualizerIterator& _Iterator, const FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> void {
        IKLDebugImGuiFeatureInterface_Unique& Interface = _Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Unique>();
        FKLDebugFeatureSceneProxyInput_Unique Input{ _RenderingComponent, _Entry.TryGetFeatureContextMutable() };
        TUniquePtr<FDebugRenderSceneProxy> SceneProxy = Interface.CreateDebugSceneProxy(Input);
        _Callback(MoveTemp(SceneProxy), Input.GetDrawDelegateHelper());
    };

    IterateOnSelectedFeaturesMutable(Callback, _FeatureContainerManager);
}

void FKLDebugImGuiFeatureVisualizer_Unique::ApplyDelegateMutable(const UniqueDelegateCallback& _Delegate, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
{
    auto Callback = [&_Delegate](FKLDebugImGuiFeatureVisualizerIterator& _Iterator, const FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> void {
        _Delegate(_Entry, _Iterator);
    };

    IterateOnSelectedFeaturesMutable(Callback, _FeatureContainerManager);
}

void FKLDebugImGuiFeatureVisualizer_Unique::TickFeatures(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem)
{
    auto Callback = [&_World, &_RequiredExternalSystem](FKLDebugImGuiFeatureVisualizerIterator& _Iterator, const FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> void {
        IKLDebugImGuiFeatureInterface_Unique& Interface = _Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_Unique>();
        FKLDebugFeatureTickInput_Unique Input{ _World, _Entry.TryGetFeatureContextMutable() };
        Interface.Tick(Input);
        KL::Debug::ImGui::Features::Types::UpdateFeatureEnableSet(KL::Debug::ImGui::Features::Types::EFeatureEnableType::UpdateSceneProxy, Input.GetUpdateFlags().IsFlagSet(FKLDebugImGuiFeatureInputFlags::EFeatureUpdateFlags::SceneProxy), _RequiredExternalSystem);
    };

    IterateOnSelectedFeaturesMutable(Callback, _FeatureContainerManager);
}