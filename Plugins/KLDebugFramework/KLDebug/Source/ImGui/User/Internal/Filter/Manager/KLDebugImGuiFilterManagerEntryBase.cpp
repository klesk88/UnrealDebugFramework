// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Filter/Manager/KLDebugImGuiFilterManagerEntryBase.h"

#include "Filter/Manager/KLDebugImGuiFilterManager.h"

FKLDebugImGuiFilterManagerEntryBase::FKLDebugImGuiFilterManagerEntryBase(const size_t _ClassSize)
{
    FKLDebugImGuiFilterManager& FilterManager = FKLDebugImGuiFilterManager::Get();
    FilterManager.AddEntry(_ClassSize, *this);
}
