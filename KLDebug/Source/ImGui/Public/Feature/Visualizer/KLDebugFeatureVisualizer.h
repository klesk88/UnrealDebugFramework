#pragma once

#include "Feature/KLDebugFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class FKLDebugImGuiFeatureContainerBase;
class UObject;

class KLDEBUGIMGUI_API FKLDebugFeatureVisualizer final : public FNoncopyable
{
public:
    explicit FKLDebugFeatureVisualizer(UObject& _Object, TArray<KL::Debug::Features::Types::FeatureIndex>&& _FeaturesIndexes);
    UE_NODISCARD bool operator==(const UObject& _Object) const;

    UE_NODISCARD bool IsValid() const;

    void DrawImGui(FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const;
    void Render(FKLDebugImGuiFeatureContainerBase& _FeatureContainer) const;

private:
    TArray<KL::Debug::Features::Types::FeatureIndex> mFeaturesIndexes;
    TWeakObjectPtr<UObject>                          mObject;
};
