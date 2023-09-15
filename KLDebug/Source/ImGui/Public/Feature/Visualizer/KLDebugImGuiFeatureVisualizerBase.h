#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTree.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class FKLDebugImGuiFeatureVisualizerImGuiContext;
class FKLDebugImGuiFeatureVisualizerRenderContext;
class UObject;
class UWorld;

class FKLDebugImGuiFeatureVisualizerBase : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerBase(const FKLDebugImGuiFeatureContainerBase& _Container, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    virtual ~FKLDebugImGuiFeatureVisualizerBase() = default;

    UE_NODISCARD virtual bool IsValid() const;

    void         DrawImGui(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context);
    virtual void Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const = 0;

protected:
    virtual void DrawImGuiTree(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) = 0;
    virtual void DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) = 0;

    template<typename CallbackType>
    void DrawImguiFeaturesEnabledCommon(FKLDebugImGuiFeatureContainerBase& _FeatureContainer, const CallbackType& _Callback);

protected:
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> mFeaturesIndexes;
    TArray<FKLDebugImGuiFeatureVisualizerEntry>             mSelectedFeaturesIndexes;
    FKLDebugImGuiFeatureVisualizerTree                      mTreeVisualizer;
};

inline bool FKLDebugImGuiFeatureVisualizerBase::IsValid() const
{
    return true;
}

template<typename CallbackType>
void FKLDebugImGuiFeatureVisualizerBase::DrawImguiFeaturesEnabledCommon(FKLDebugImGuiFeatureContainerBase& _FeatureContainer, const CallbackType& _Callback)
{
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex, TInlineAllocator<30>> FeaturesToRemove;

    FKLDebugImGuiFeatureVisualizerIterator Iterator = _FeatureContainer.GetFeatureVisualizerIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        if (!_Callback(Iterator, Iterator.GetEntryDataMutable()))
        {
            FeaturesToRemove.Emplace(Iterator.GetFeatureDataIndex());
        }
    }

    for (int32 i = FeaturesToRemove.Num() - 1; i >= 0; --i)
    {
        const KL::Debug::ImGui::Features::Types::FeatureIndex Index = FeaturesToRemove[i];
        const FKLDebugImGuiFeatureVisualizerEntry& Entry = mSelectedFeaturesIndexes[Index];
        mTreeVisualizer.ClearToogleNodeData(Entry.GetNodeDataID());
        mSelectedFeaturesIndexes.RemoveAtSwap(Index, 1, false);
    }
}
