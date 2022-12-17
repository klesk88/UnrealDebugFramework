#include "KLDebugRuntimeModule.h"

#include "Window/Manager/KLDebugWindowManager.h"

// engine
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLDebugRuntime"

namespace KL::Debug::RuntimeModuleHelpers
{
    static const FName KLDebugRuntimeModuleName(TEXT("KLDebugRuntime"));
}  // namespace KL::Debug::RuntimeModuleHelpers

////////////////////////////////////////////////////////////////////////////

void FKLDebugRuntimeModule::StartupModule()
{
}

void FKLDebugRuntimeModule::ShutdownModule()
{
}

FKLDebugRuntimeModule& FKLDebugRuntimeModule::Get()
{
    return FModuleManager::GetModuleChecked<FKLDebugRuntimeModule>(KL::Debug::RuntimeModuleHelpers::KLDebugRuntimeModuleName);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugRuntimeModule, KLDebugRuntime)