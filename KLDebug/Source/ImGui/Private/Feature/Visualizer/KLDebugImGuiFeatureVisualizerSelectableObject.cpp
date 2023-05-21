#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerSelectableObject.h"

#include "Feature/Container/Iterators/KLDebugImGuiSubsetFeaturesIterator.h"
#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"

// ImGuiThirdParty module
#include "ThirdParty/ImGuiThirdParty/Public/Library/imgui.h"

// engine
#include "Containers/UnrealString.h"

FKLDebugImGuiFeatureVisualizerSelectableObject::FKLDebugImGuiFeatureVisualizerSelectableObject(const FKLDebugImGuiFeatureContainerBase& _Container, UObject& _Object, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes)
    : FKLDebugImGuiFeatureVisualizerBase(_Container, MoveTemp(_FeaturesIndexes))
    , mObject(&_Object)
{
}

void FKLDebugImGuiFeatureVisualizerSelectableObject::DrawImGuiTree(const UWorld& _World)
{
#if DO_ENSURE
    if (!IsValid())
    {
        ensureMsgf(false, TEXT("should be valid when reached here"));
        return;
    }
#endif

    const FString ObjectName = mObject->GetFName().ToString();
    if (!ImGui::TreeNode(TCHAR_TO_ANSI(*ObjectName)))
    {
        ImGui::SameLine();
        if (ImGui::Button("Remove From List"))
        {
            mKeepAlive = false;
        }

        return;
    }

    if (ImGui::Button("Remove From List"))
    {
        mKeepAlive = false;
        return;
    }

    mTreeVisualizer.DrawImGuiTree(mSelectedFeaturesIndexes);

    ImGui::TreePop();
}

void FKLDebugImGuiFeatureVisualizerSelectableObject::DrawImGuiFeaturesEnabled(const UWorld& _World, FKLDebugImGuiFeatureContainerBase& _FeatureContainer)
{
#if DO_ENSURE
    if (!IsValid())
    {
        ensureMsgf(false, TEXT("should be valid when reached here"));
        return;
    }
#endif

    UObject& Object = *mObject.Get();

    auto Callback = [&_World, &Object](FKLDebugImGuiFeatureVisualizerIterator& Iterator, FKLDebugImGuiFeatureVisualizerEntry& _Entry) -> bool{
        IKLDebugImGuiFeatureInterface_SelectableObject& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterface_SelectableObject>();
        Interface.DrawImGui(_World, _Entry.GetIsEnableRef(), Object);
        return _Entry.IsEnable();
    };

    DrawImguiFeaturesEnabledCommon(_FeatureContainer, Callback);
}

void FKLDebugImGuiFeatureVisualizerSelectableObject::Render(const UWorld& _World, FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const
{
#if DO_ENSURE
    if (!IsValid())
    {
        ensureMsgf(false, TEXT("should be valid when reached here"));
        return;
    }
#endif

    const UObject&                           Object   = *mObject.Get();
    FKLDebugImGuiFeatureVisualizerConstIterator Iterator = _FeatureContainer.GetFeatureVisualizerConstIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        const IKLDebugImGuiFeatureInterface_SelectableObject& Interface = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterface_SelectableObject>();
        Interface.Render(Object, _World);
    }
}