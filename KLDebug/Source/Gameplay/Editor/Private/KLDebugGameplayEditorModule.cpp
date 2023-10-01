#include "KLDebugGameplayEditorModule.h"

// engine
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLDebugGameplayEditor"

namespace KL::Debug::GameplayEditor::Module
{
    static const FName Module(TEXT("KLDebugGameplayEditor"));

}  // namespace  KL::Debug::GameplayEditor::Module

////////////////////////////////////////////////////////////////////////////

void FKLDebugGameplayEditorModule::StartupModule()
{
    mInputManager.Init();
}

void FKLDebugGameplayEditorModule::ShutdownModule()
{
    mInputManager.Shutdown();
}

FKLDebugGameplayEditorModule& FKLDebugGameplayEditorModule::Get()
{
    return FModuleManager::GetModuleChecked<FKLDebugGameplayEditorModule>(KL::Debug::GameplayEditor::Module::Module);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugGameplayEditorModule, KLDebugGameplayEditor)