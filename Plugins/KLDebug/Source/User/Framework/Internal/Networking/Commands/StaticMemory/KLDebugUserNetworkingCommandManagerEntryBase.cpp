// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Networking/Commands/StaticMemory/KLDebugUserNetworkingCommandManagerEntryBase.h"

#include "Internal/Networking/Commands/StaticMemory/KLDebugUserNetworkingCommandManager.h"

FKLDebugUserNetworkingCommandManagerEntryBase::FKLDebugUserNetworkingCommandManagerEntryBase(const SIZE_T _ClassSize)
{
    FKLDebugUserNetworkingCommandManager& CommandManager = FKLDebugUserNetworkingCommandManager::Get();
    CommandManager.AddEntry(_ClassSize, *this);
}