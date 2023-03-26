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

class KLDEBUGIMGUI_API FKLDebugImGuiFeatureVisualizer final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureVisualizer(UObject& _Object, TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    UE_NODISCARD bool operator==(const UObject& _Object) const;

    UE_NODISCARD bool IsValid() const;

    void DrawImGuiTree();
    void DrawImGui(FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const;
    void Render(FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const;

private:
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> mFeaturesIndexes;
    FKLDebugImGuiFeatureVisualizerTree                      mTreeVisualizer;
    TWeakObjectPtr<UObject>                                 mObject;
};
