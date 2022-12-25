#include "KLDebugImGuiEditorModule.h"

#include "Config/KLDebugImGuiEditorConfig.h"

// engine
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLDebugImGui"

namespace KL::Debug::ImGuiEditor::Module
{
    static const FName Module(TEXT("KLDebugImGui"));

}  // namespace  KL::Debug::ImGuiEditor::Module

////////////////////////////////////////////////////////////////////////////

void FKLDebugImGuiEditorModule::StartupModule()
{
    mInputManager.Init();
    mMainImGuiWindow.Init();
}

void FKLDebugImGuiEditorModule::ShutdownModule()
{
    mMainImGuiWindow.Shutdown();
    mInputManager.Shutdown();
}

FKLDebugImGuiEditorModule& FKLDebugImGuiEditorModule::Get()
{
    return FModuleManager::GetModuleChecked<FKLDebugImGuiEditorModule>(KL::Debug::ImGuiEditor::Module::Module);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugImGuiEditorModule, KLDebugImGuiEditor)