#include "KLDebugViewportModule.h"

// engine
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

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugViewportModule, KLDebugViewport)