#include "Picker/KLDebugImGuiEditorPicker.h"

//KLDebugImGui module
#include "ImGui/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
//KLUnrealImGui module
#include "UnrealImGui/Public/KLUnrealImGuiModule.h"

//engine
#include "Engine/Selection.h"
#include "UObject/Object.h"

void FKLDebugImGuiEditorPicker::Init()
{
    FKLUnrealImGuiModule& ImGuiModule = FKLUnrealImGuiModule::Get();
    mOnImGuiStateChangeHandle = ImGuiModule.BindToEnableStateChange(FOnNetImguiEnableStateChange::FDelegate::CreateRaw(this, &FKLDebugImGuiEditorPicker::OnImGuiEnable));
}

void FKLDebugImGuiEditorPicker::Shutdown()
{
    FKLUnrealImGuiModule* ImGuiModule = FKLUnrealImGuiModule::TryGet();
    if (ImGuiModule && mOnImGuiStateChangeHandle.IsValid())
    {
        ImGuiModule->UnBindFromEnableStateChange(mOnImGuiStateChangeHandle);
    }

    Clear();
}

void FKLDebugImGuiEditorPicker::Clear()
{
    if (mOnSelectionHandle.IsValid())
    {
        USelection::SelectObjectEvent.Remove(mOnSelectionHandle);
        mOnSelectionHandle.Reset();
    }
}

void FKLDebugImGuiEditorPicker::OnImGuiEnable(const bool _IsEnable)
{
    if (_IsEnable)
    {
        mOnSelectionHandle = USelection::SelectObjectEvent.AddRaw(this, &FKLDebugImGuiEditorPicker::OnSelection);
    }
    else
    {
        Clear();
    }
}

void FKLDebugImGuiEditorPicker::OnSelection(UObject* _Object) const
{
    if (!_Object)
    {
        return;
    }

    UKLDebugImGuiEngineSubsystem* ImGuiEngineSubsystem = UKLDebugImGuiEngineSubsystem::Get();
    if (!ImGuiEngineSubsystem)
    {
        return;
    }

    ImGuiEngineSubsystem->OnObjectSelect(*_Object);
}
