#include "KLDebugViewportModule.h"

#include "Viewport/KLDebugViewportClient.h"

// engine
#include "Framework/Application/SlateApplication.h"
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLDebugViewport"

namespace KL::Debug::Viewport::Module
{
    static const FName Module(TEXT("KLDebugViewport"));
}  // namespace  KL::Debug::Viewport::Module

////////////////////////////////////////////////////////////////////////////

void FKLDebugViewportModule::StartupModule()
{
}

void FKLDebugViewportModule::ShutdownModule()
{
}

FKLDebugViewportModule& FKLDebugViewportModule::Get()
{
    return FModuleManager::GetModuleChecked<FKLDebugViewportModule>(KL::Debug::Viewport::Module::Module);
}

void FKLDebugViewportModule::ActivateDebugViewport()
{
    //FSlateApplication& SlateApplication = FSlateApplication::Get();
    //mDebugViewportClient                = MakeShared<UKLDebugViewportClient>();
    //SlateApplication.RegisterGameViewport()
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugViewportModule, KLDebugViewport)