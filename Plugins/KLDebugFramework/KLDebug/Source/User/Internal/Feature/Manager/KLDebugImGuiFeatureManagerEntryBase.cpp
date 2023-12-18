// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

#include "Feature/Manager/KLDebugImGuiFeatureManager.h"

FKLDebugImGuiFeatureManagerEntryBase::FKLDebugImGuiFeatureManagerEntryBase(const size_t _ClassSize)
{
    FKLDebugImGuiFeatureManager& WindowManager = FKLDebugImGuiFeatureManager::Get();
    WindowManager.AddEntry(_ClassSize, *this);
}
