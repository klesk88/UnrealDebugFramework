#pragma once

#include "Features/Engine/Picker/KLDebugUtilsPicker.h"

// imgui module
#include "ImGui/Public/Feature/Interface/Selectable/KLDebugImGuiFeatureInterface_SelectableObject.h"

//engine
#include "Containers/Array.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

class AActor;
class FProperty;
class UWorld;

class KLDEBUGEXAMPLES_API FKLDebugFeatureObject_PropertyWatcher : public IKLDebugImGuiFeatureInterface_SelectableObject
{
public:
    //IKLDebugImGuiFeatureInterface_SelectableObject
    UE_NODISCARD bool DoesSupportObject(const UObject& _Object) const final;
    //IKLDebugImGuiFeatureInterface_SelectableObject

private:
    //IKLDebugImGuiFeatureInterface_SelectableObject
    void                        DrawImGuiChild(const FKLDebugImGuiFeatureInterfaceImGuiContext_Selectable& _Context) final;
    UE_NODISCARD const FString& GetWindowName() const final;
    void                        GetFilterPath(TArray<FName>& _OutFilters) const final;
    UE_NODISCARD const FName&   GetImGuiPath() const final;
    //IKLDebugImGuiFeatureInterface_SelectableObject

    void ImguiDrawProperty(FProperty& _Property, TArray<char>& _StringBuffer, UObject& _Object) const;
};
