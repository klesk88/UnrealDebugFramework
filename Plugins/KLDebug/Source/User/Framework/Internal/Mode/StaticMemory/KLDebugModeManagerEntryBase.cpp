// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Mode//StaticMemory/KLDebugModeManagerEntryBase.h"

#include "Internal/Mode//StaticMemory/KLDebugModeManager.h"

FKLDebugModeManagerEntryBase::FKLDebugModeManagerEntryBase(const SIZE_T _ClassSize)
{
    FKLDebugModeManager& CommandManager = FKLDebugModeManager::Get();
    CommandManager.AddEntry(_ClassSize, *this);
}