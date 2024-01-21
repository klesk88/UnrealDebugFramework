// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Feature/Manager/KLDebugFeatureManagerEntryBase.h"

#include "Internal/Feature/Manager/KLDebugFeatureManager.h"

FKLDebugFeatureManagerEntryBase::FKLDebugFeatureManagerEntryBase(const SIZE_T _ClassSize)
{
    FKLDebugFeatureManager& WindowManager = FKLDebugFeatureManager::Get();
    WindowManager.AddEntry(_ClassSize, *this);
}
