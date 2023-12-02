// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "KLDebugGameplayRuntimeModule.h"

#include "Config/KLDebugGameplayConfig.h"

// engine
#include "Modules/ModuleManager.h"
#include "UObject/NameTypes.h"

#define LOCTEXT_NAMESPACE "KLDebugGameplayRuntimeModule"

namespace KL::Debug::Gameplay::Module
{
    static const FName Module(TEXT("KLDebugGameplayRuntimeModule"));

}    // namespace KL::Debug::Gameplay::Module

////////////////////////////////////////////////////////////////////////////

FKLDebugGameplayRuntimeModule& FKLDebugGameplayRuntimeModule::GetMutable()
{
    return FModuleManager::GetModuleChecked<FKLDebugGameplayRuntimeModule>(KL::Debug::Gameplay::Module::Module);
}

void FKLDebugGameplayRuntimeModule::StartupModule()
{
    UKLDebugGameplayConfig::Get().Load();
}

void FKLDebugGameplayRuntimeModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKLDebugGameplayRuntimeModule, KLDebugGameplayRuntime)
