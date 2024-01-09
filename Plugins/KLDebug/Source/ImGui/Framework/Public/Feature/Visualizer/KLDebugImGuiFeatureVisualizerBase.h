// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Container/KLDebugImGuiFeatureContainerBase.h"
#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Feature/Delegates/KLDebugImGuiFeatureStatusUpdateData.h"
#include "Feature/Helpers/KLDebugFrameworkFeatureHelpers.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerImGuiContext.h"
#include "Feature/Visualizer/Context/KLDebugImGuiFeatureVisualizerRenderContext.h"
#include "Feature/Visualizer/KLDebugImGuiFeatureVisualizerEntry.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTree.h"
#include "Rendering/KLDebugFrameworkRenderingDefinitions.h"

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FKLDebugImGuiFeaturesTypesContainerManager;
class FKLDebugImGuiFeatureVisualizerRenderContext;
class FKLDebugFeatureDrawCanvasInput_Base;
class IKLDebugImGuiFeatureInterfaceBase;
class UCanvas;
class UFont;
class UObject;
class UPrimitiveComponent;
class UWorld;

class FKLDebugImGuiFeatureVisualizerBase : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerBase() = default;
    explicit FKLDebugImGuiFeatureVisualizerBase(const FKLDebugImGuiFeatureContainerBase& _Container, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    virtual ~FKLDebugImGuiFeatureVisualizerBase() = default;

    virtual void TickFeatures(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, KL::Debug::ImGui::Features::Types::FeatureEnableSet& _RequiredExternalSystem) = 0;
    virtual void DrawOnCanvas(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, UCanvas& _Canvas, UFont& _Font, UWorld& _World) const = 0;
    virtual void Render(const FKLDebugImGuiFeatureVisualizerRenderContext& _Context) const = 0;
    virtual void GatherSceneProxies(const UPrimitiveComponent& _RenderingComponent, const KL::Debug::Framework::Rendering::GatherSceneProxyCallback& _Callback, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager) = 0;

    UE_NODISCARD virtual bool IsValid() const;

    void Init(const FKLDebugImGuiFeatureContainerBase& _Container, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    void Shutdown(const UWorld& _World, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, UObject* _OwnerObject);

    void DrawImGui(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context);
    UE_NODISCARD const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& GetFeaturesIndexes() const;

    UE_NODISCARD const FKLDebugImGuiFeatureVisualizerEntry* TryGetSelectedFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const;
    UE_NODISCARD EImGuiInterfaceType GetInterfaceType() const;

protected:
    virtual void DrawImGuiTree(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) = 0;
    virtual void DrawImGuiFeaturesEnabled(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context) = 0;

    template <typename CallbackType>
    void DrawImguiFeaturesEnabledCommon(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context, const CallbackType& _Callback, UObject* _OwnerObject);

    template <typename CallbackType>
    void IterateOnSelectedFeatures(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const CallbackType& _Callback) const;

    template <typename CallbackType>
    void IterateOnSelectedFeaturesMutable(const CallbackType& _Callback, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager);

    static void InitCommonCanvasInput(UWorld& _World, FKLDebugFeatureDrawCanvasInput_Base& _Input);

protected:
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> mFeaturesIndexes;
    TArray<FKLDebugImGuiFeatureVisualizerEntry> mSelectedFeaturesIndexes;
    FKLDebugImGuiFeatureVisualizerTree mTreeVisualizer;
    EImGuiInterfaceType mInterfaceType = EImGuiInterfaceType::COUNT;
};

inline bool FKLDebugImGuiFeatureVisualizerBase::IsValid() const
{
    return mInterfaceType != EImGuiInterfaceType::COUNT;
}

template <typename CallbackType>
void FKLDebugImGuiFeatureVisualizerBase::DrawImguiFeaturesEnabledCommon(const FKLDebugImGuiFeatureVisualizerImGuiContext& _Context, const CallbackType& _Callback, UObject* _OwnerObject)
{
    TArray<uint32> FeaturesToRemoveIdx;
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> FeaturesToRemove;
    FeaturesToRemove.Reserve(mSelectedFeaturesIndexes.Num());
    FeaturesToRemoveIdx.Reserve(mSelectedFeaturesIndexes.Num());

    FKLDebugImGuiFeatureContainerBase& FeatureContainer = _Context.GetFeaturesContainerManager().GetContainerMutable(mInterfaceType);
    FKLDebugImGuiFeatureVisualizerIterator Iterator = FeatureContainer.GetFeatureVisualizerIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        const IKLDebugImGuiFeatureInterfaceBase& InterfaceConst = Iterator.GetFeatureInterfaceCasted<IKLDebugImGuiFeatureInterfaceBase>();
        if (!InterfaceConst.RequireDrawImGui())
        {
            continue;
        }

        if (!_Callback(Iterator, Iterator.GetEntryDataMutable()))
        {
            FeaturesToRemoveIdx.Emplace(Iterator.GetIteratorIndex());
            FeaturesToRemove.Emplace(Iterator.GetFeatureDataIndex());

            IKLDebugImGuiFeatureInterfaceBase& Interface = Iterator.GetFeatureInterfaceCastedMutable<IKLDebugImGuiFeatureInterfaceBase>();
            KL::Debug::Feature::Helpers::OnFeatureUnselected(_Context.GetWorld(), _OwnerObject, Interface);
        }
    }

    for (int32 i = FeaturesToRemoveIdx.Num() - 1; i >= 0; --i)
    {
        const uint32 Index = FeaturesToRemoveIdx[i];
        const FKLDebugImGuiFeatureVisualizerEntry& Entry = mSelectedFeaturesIndexes[Index];
        mTreeVisualizer.ClearToogleNodeData(Entry.GetNodeDataID());
        mSelectedFeaturesIndexes.RemoveAtSwap(Index, 1, false);
    }

    if (_Context.GetFeatureUpdateDelegate().IsBound() && !FeaturesToRemove.IsEmpty())
    {
        FKLDebugImGuiSubsetFeaturesConstIterator RemovedIterator = FeatureContainer.GetFeaturesSubsetConstIterator(FeaturesToRemove);
        FKLDebugImGuiFeatureStatusUpdateData DelegateData{ _Context.GetWorld(), false, mInterfaceType, _OwnerObject, RemovedIterator };
        if (mSelectedFeaturesIndexes.IsEmpty())
        {
            DelegateData.SetFullyRemoved();
        }

        _Context.GetFeatureUpdateDelegate().Broadcast(DelegateData);
    }
}

template <typename CallbackType>
void FKLDebugImGuiFeatureVisualizerBase::IterateOnSelectedFeatures(const FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager, const CallbackType& _Callback) const
{
    const FKLDebugImGuiFeatureContainerBase& FeatureContainer = _FeatureContainerManager.GetContainer(mInterfaceType);
    FKLDebugImGuiFeatureVisualizerConstIterator Iterator = FeatureContainer.GetFeatureVisualizerConstIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        _Callback(Iterator, Iterator.GetEntryData());
    }
}

template <typename CallbackType>
void FKLDebugImGuiFeatureVisualizerBase::IterateOnSelectedFeaturesMutable(const CallbackType& _Callback, FKLDebugImGuiFeaturesTypesContainerManager& _FeatureContainerManager)
{
    FKLDebugImGuiFeatureContainerBase& FeatureContainer = _FeatureContainerManager.GetContainerMutable(mInterfaceType);
    FKLDebugImGuiFeatureVisualizerIterator Iterator = FeatureContainer.GetFeatureVisualizerIterator(mSelectedFeaturesIndexes);
    for (; Iterator; ++Iterator)
    {
        _Callback(Iterator, Iterator.GetEntryDataMutable());
    }
}

inline const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& FKLDebugImGuiFeatureVisualizerBase::GetFeaturesIndexes() const
{
    return mFeaturesIndexes;
}

inline const FKLDebugImGuiFeatureVisualizerEntry* FKLDebugImGuiFeatureVisualizerBase::TryGetSelectedFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const
{
    return mSelectedFeaturesIndexes.FindByKey(_FeatureIndex);
}

inline EImGuiInterfaceType FKLDebugImGuiFeatureVisualizerBase::GetInterfaceType() const
{
    return mInterfaceType;
}