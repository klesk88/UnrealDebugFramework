#pragma once

#include "Feature/KLDebugImGuiFeatureTypes.h"
#include "Feature/Visualizer/Tree/KLDebugImGuiFeatureVisualizerTree.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FKLDebugImGuiFeatureContainerBase;
class UObject;
class UWorld;

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizerBase : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizerBase(const FKLDebugImGuiFeatureContainerBase& _Container, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    virtual ~FKLDebugImGuiFeatureVisualizerBase() = default;

    UE_NODISCARD virtual bool IsValid() const;

    void         DrawImGui(const UWorld& _World, FKLDebugImGuiFeatureContainerBase& _FeatureContainer);
    virtual void Render(const UWorld& _World, FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const = 0;

protected:
    virtual void DrawImGuiTree(const UWorld& _World)                                                                        = 0;
    virtual void DrawImGuiFeaturesEnabled(const UWorld& _World, FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const = 0;

protected:
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> mFeaturesIndexes;
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> mSelectedFeaturesIndexes;
    FKLDebugImGuiFeatureVisualizerTree                      mTreeVisualizer;
};

inline bool FKLDebugImGuiFeatureVisualizerBase::IsValid() const
{
    return true;
}
