#pragma once

#include "Features/Engine/Picker/KLDebugUtilsPicker.h"

// imgui user module
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_Selectable.h"

//engine
#include "Containers/Array.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AActor;
class FKLDebugImGuiFeatureImGuiInput_Selectable;
class FProperty;
class UWorld;

class KLDEBUGEXAMPLES_API FKLDebugFeatureObject_PropertyWatcher : public IKLDebugImGuiFeatureInterface_Selectable
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureObject_PropertyWatcher, IKLDebugImGuiFeatureInterface_Selectable)

private:
    //IKLDebugImGuiFeatureInterface_Selectable
    void                        DrawImGuiChild(const FKLDebugImGuiFeatureImGuiInput_Selectable& _Input) final;
    UE_NODISCARD const FString& GetWindowName() const final;
    void                        GetFilterPath(TArray<FName>& _OutFilters) const final;
    UE_NODISCARD const FName&   GetImGuiPath() const final;
    //IKLDebugImGuiFeatureInterface_Selectable

    void ImguiDrawProperty(FProperty& _Property, TArray<char>& _StringBuffer, UObject& _Object) const;
};
