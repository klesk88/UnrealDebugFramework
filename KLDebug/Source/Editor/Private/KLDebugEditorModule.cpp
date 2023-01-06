#include "KLDebugEditorModule.h"

// engine
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLDebugGameplayEditor"

namespace KL::Debug::GameplayEditor::Module
{
    static const FName Module(TEXT("KLDebugEditor"));

}  // namespace  KL::Debug::GameplayEditor::Module

////////////////////////////////////////////////////////////////////////////

void KLDebugEditorModule::StartupModule()
{
    mInputManager.Init();
    mMainImGuiWindow.Init();
    mEditorPicker.Init();
}

void KLDebugEditorModule::ShutdownModule()
{
    mMainImGuiWindow.Shutdown();
    mInputManager.Shutdown();
    mEditorPicker.Shutdown();
}

KLDebugEditorModule& KLDebugEditorModule::Get()
{
    return FModuleManager::GetModuleChecked<KLDebugEditorModule>(KL::Debug::GameplayEditor::Module::Module);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(KLDebugEditorModule, KLDebugEditor)