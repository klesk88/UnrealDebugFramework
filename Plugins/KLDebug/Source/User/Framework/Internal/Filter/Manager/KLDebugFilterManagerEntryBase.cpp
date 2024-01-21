// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Filter/Manager/KLDebugFilterManagerEntryBase.h"

#include "Internal/Filter/Manager/KLDebugFilterManager.h"

FKLDebugFilterManagerEntryBase::FKLDebugFilterManagerEntryBase(const SIZE_T _ClassSize)
{
    FKLDebugFilterManager& FilterManager = FKLDebugFilterManager::Get();
    FilterManager.AddEntry(_ClassSize, *this);
}
