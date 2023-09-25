#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
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
class FKLDebugImGuiFeatureVisualizerRenderContext;
class UObject;
class UWorld;

class FKLDebugImGuiFeatureVisualizerBase : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerBase(const FKLDebugImGuiFeatureContainerBase& _Container, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    virtual ~FKLDebugImGuiFeatureVisualizerBase() = default;

    UE_NODISCARD virtual bool IsValid() const;
    virtual void Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const = 0;

    void DrawImGui(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context);
    UE_NODISCARD const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& GetFeaturesIndexes() const;

protected:
    virtual void DrawImGuiTree(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) = 0;
    virtual void DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) = 0;

    template<typename CallbackType>
    void DrawImguiFeaturesEnabledCommon(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context, const EContainerType _ContainerType, const UObject& _OwnerObject, const CallbackType& _Callback);

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
void FKLDebugImGuiFeatureVisualizerBase::DrawImguiFeaturesEnabledCommon(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context, const EContainerType _ContainerType, const UObject& _OwnerObject, const CallbackType& _Callback)
{
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> FeaturesToRemove;
    FeaturesToRemove.Reserve(30);

    FKLDebugImGuiFeatureContainerBase& FeatureContainer = _Context.GetFeaturesContainerManager().GetContainerMutable(_ContainerType);
    FKLDebugImGuiFeatureVisualizerIterator Iterator = FeatureContainer.GetFeatureVisualizerIterator(mSelectedFeaturesIndexes);
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

    if (_Context.GetFeatureUpdateDelegate().IsBound() && !FeaturesToRemove.IsEmpty())
    {
        FKLDebugImGuiSubsetFeaturesConstIterator RemovedIterator = FeatureContainer.GetFeaturesSubsetConstIterator(FeaturesToRemove);
        FKLDebugImGuiFeatureStatusUpdateData DelegateData{ false, _ContainerType, _OwnerObject, RemovedIterator };
        if (mSelectedFeaturesIndexes.IsEmpty())
        {
            DelegateData.SetFullyRemoved();
        }

        _Context.GetFeatureUpdateDelegate().Broadcast(DelegateData);
    }
}

inline const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& FKLDebugImGuiFeatureVisualizerBase::GetFeaturesIndexes() const
{
    return mFeaturesIndexes;
}