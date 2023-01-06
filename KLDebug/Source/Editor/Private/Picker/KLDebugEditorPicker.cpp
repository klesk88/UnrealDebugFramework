#include "Picker/KLDebugEditorPicker.h"

// KLDebugImGui module
#include "ImGui/Public/Subsystems/KLDebugImGuiEngineSubsystem.h"
// KLUnrealImGui module
#include "UnrealImGui/Public/KLUnrealImGuiModule.h"

// engine
#include "Engine/Selection.h"
#include "UObject/Object.h"

void FKLDebugEditorPicker::Init()
{
    FKLUnrealImGuiModule& ImGuiModule = FKLUnrealImGuiModule::Get();
    mOnImGuiStateChangeHandle         = ImGuiModule.BindToEnableStateChange(FOnNetImguiEnableStateChange::FDelegate::CreateRaw(this, &FKLDebugEditorPicker::OnImGuiEnable));
}

void FKLDebugEditorPicker::Shutdown()
{
    FKLUnrealImGuiModule* ImGuiModule = FKLUnrealImGuiModule::TryGet();
    if (ImGuiModule && mOnImGuiStateChangeHandle.IsValid())
    {
        ImGuiModule->UnBindFromEnableStateChange(mOnImGuiStateChangeHandle);
    }

    Clear();
}

void FKLDebugEditorPicker::Clear()
{
    if (mOnSelectionHandle.IsValid())
    {
        USelection::SelectObjectEvent.Remove(mOnSelectionHandle);
        mOnSelectionHandle.Reset();
    }
}

void FKLDebugEditorPicker::OnImGuiEnable(const bool _IsEnable)
{
    if (_IsEnable)
    {
        mOnSelectionHandle = USelection::SelectObjectEvent.AddRaw(this, &FKLDebugEditorPicker::OnSelection);
    }
    else
    {
        Clear();
    }
}

void FKLDebugEditorPicker::OnSelection(UObject* _Object) const
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
