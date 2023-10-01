#pragma once

#include "Features/Engine/Picker/KLDebugUtilsPicker.h"

// imgui user module
#include "ImGui/User/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"

//engine
#include "Containers/Array.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AActor;
class FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable;
class FProperty;
class UWorld;

class KLDEBUGEXAMPLES_API FKLDebugFeatureObject_PropertyWatcher : public IKLDebugImGuiFeatureInterface_SelectableObject
{
    DERIVED_KL_DEBUG_FEATURE_CLASS(FKLDebugFeatureObject_PropertyWatcher, IKLDebugImGuiFeatureInterface_SelectableObject)

private:
    //IKLDebugImGuiFeatureInterface_SelectableObject
    void                        DrawImGuiChild(const FKLDebugImGuiFeatureInterfaceImGuiInput_Selectable& _Input) final;
    UE_NODISCARD const FString& GetWindowName() const final;
    void                        GetFilterPath(TArray<FName>& _OutFilters) const final;
    UE_NODISCARD const FName&   GetImGuiPath() const final;
    //IKLDebugImGuiFeatureInterface_SelectableObject

    void ImguiDrawProperty(FProperty& _Property, TArray<char>& _StringBuffer, UObject& _Object) const;
};
