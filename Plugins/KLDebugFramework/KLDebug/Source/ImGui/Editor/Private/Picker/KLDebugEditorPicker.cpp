// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Picker/KLDebugEditorPicker.h"

// modules
#include "ImGui/Framework/Public/Subsystems/KLDebugImGuiWorldSubsystem.h"
#include "UnrealImGui/Public/KLUnrealImGuiModule.h"

// engine
#include "Engine/Selection.h"
#include "UObject/Object.h"

void FKLDebugEditorPicker::Init()
{
    FKLUnrealImGuiModule& ImGuiModule = FKLUnrealImGuiModule::Get();
    mOnImGuiStateChangeHandle = ImGuiModule.BindToEnableStateChange(FOnNetImguiEnableStateChange::FDelegate::CreateRaw(this, &FKLDebugEditorPicker::OnImGuiEnable));
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

    UKLDebugImGuiWorldSubsystem* ImGuiWorldSubsystem = UKLDebugImGuiWorldSubsystem::TryGetMutable(*_Object);
    if (ImGuiWorldSubsystem)
    {
        ImGuiWorldSubsystem->OnObjectSelected(*_Object);
    }
}
