#include "KLDebugEditorViewportModule.h"

#include "Commands/KLDebugEditorViewportCommands.h"
#include "Layout/KLDebugEditorViewportLayoutEntity.h"

// engine
#include "Framework/Application/SlateApplication.h"
#include "LevelEditor.h"
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLDebugEditorViewport"

namespace KL::Debug::EditorViewport::Module
{
    static const FName Module(TEXT("KLDebugEditorViewport"));
    static const FName Viewport(TEXT("KLDebugViewport"));
}  // namespace  KL::Debug::Viewport::Module

////////////////////////////////////////////////////////////////////////////

void FKLDebugEditorViewportModule::StartupModule()
{
}

void FKLDebugEditorViewportModule::ShutdownModule()
{
}

FKLDebugEditorViewportModule& FKLDebugEditorViewportModule::Get()
{
    return FModuleManager::GetModuleChecked<FKLDebugEditorViewportModule>(KL::Debug::EditorViewport::Module::Module);
}

void FKLDebugEditorViewportModule::ActivateDebugViewport()
{
    //FSlateApplication& SlateApplication = FSlateApplication::Get();
    //mDebugViewportClient                = MakeShared<UKLDebugViewportClient>();
    //SlateApplication.RegisterGameViewport()
}

void FKLDebugEditorViewportModule::RegisterMenuExtensions()
{
    FKLDebugEditorViewportCommands::Register();
}

void FKLDebugEditorViewportModule::RegisterLevelEditorExtensions()
{
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

   // FViewportTypeDefinition DebugViewportType = FViewportTypeDefinition::FromType<FKLDebugEditorViewportLayoutEntity>(FKLDebugEditorViewportCommands::Get().GetViewportCommand());
    //LevelEditorModule.RegisterViewportType(KL::Debug::EditorViewport::Module::Viewport, DebugViewportType);
}

void FKLDebugEditorViewportModule::UnregisterLevelEditorExtensions()
{
    FLevelEditorModule* LevelEditorModule = FModuleManager::GetModulePtr<FLevelEditorModule>("LevelEditor");
    if (LevelEditorModule)
    {
        LevelEditorModule->UnregisterViewportType(KL::Debug::EditorViewport::Module::Viewport);
    }
}

void FKLDebugEditorViewportModule::UnregisterMenuExtensions()
{
    FKLDebugEditorViewportCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugEditorViewportModule, KLDebugEditorViewport)