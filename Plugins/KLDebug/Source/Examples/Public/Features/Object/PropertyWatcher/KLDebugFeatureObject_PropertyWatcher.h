// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Features/Engine/Picker/KLDebugUtilsPicker.h"

// imgui user module
#include "User/Framework/Public/Feature/Interface/Selectable/KLDebugFeatureInterface_Selectable.h"

// engine
#include "Containers/Array.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AActor;
class FKLDebugFeatureImGuiInput_Selectable;
class FProperty;
class UWorld;

class KLDEBUGEXAMPLES_API FKLDebugFeatureObject_PropertyWatcher : public IKLDebugFeatureInterface_Selectable
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureObject_PropertyWatcher, IKLDebugFeatureInterface_Selectable)

private:
    // IKLDebugFeatureInterface_Selectable
    void DrawImGuiChild(FKLDebugFeatureImGuiInput_Selectable& _Input) final;
    UE_NODISCARD const FString& GetWindowName() const final;
    void GetFilterPath(TArray<FName>& _OutFilters) const final;
    UE_NODISCARD const FName& GetImGuiPath() const final;
    // IKLDebugFeatureInterface_Selectable

    void ImguiDrawProperty(FProperty& _Property, TArray<char>& _StringBuffer, UObject& _Object) const;
};
