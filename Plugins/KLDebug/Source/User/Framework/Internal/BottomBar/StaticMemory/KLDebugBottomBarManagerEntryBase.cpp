// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/BottomBar/StaticMemory/KLDebugBottomBarManagerEntryBase.h"

#include "Internal/BottomBar/StaticMemory/KLDebugBottomBarManager.h"

FKLDebugBottomBarManagerEntryBase::FKLDebugBottomBarManagerEntryBase(const SIZE_T _ClassSize)
{
    FKLDebugBottomBarManager& CommandManager = FKLDebugBottomBarManager::Get();
    CommandManager.AddEntry(_ClassSize, *this);
}