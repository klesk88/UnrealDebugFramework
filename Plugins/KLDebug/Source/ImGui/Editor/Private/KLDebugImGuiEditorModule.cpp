// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "KLDebugImGuiEditorModule.h"

// engine
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLDebugImGuiEditorModule"

namespace KL::Debug::ImGuiEditor::Module
{
    static const FName Module(TEXT("KLDebugImGuiEditor"));

}    // namespace KL::Debug::ImGuiEditor::Module

////////////////////////////////////////////////////////////////////////////

void FKLDebugImGuiEditorModule::StartupModule()
{
    mEditorPicker.Init();
    mMenuExtender.Init();
}

void FKLDebugImGuiEditorModule::ShutdownModule()
{
    mMenuExtender.Shutdown();
    mEditorPicker.Shutdown();
}

FKLDebugImGuiEditorModule& FKLDebugImGuiEditorModule::Get()
{
    return FModuleManager::GetModuleChecked<FKLDebugImGuiEditorModule>(KL::Debug::ImGuiEditor::Module::Module);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugImGuiEditorModule, KLDebugImGuiEditor)