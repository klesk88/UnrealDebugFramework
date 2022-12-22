#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

#include "Feature/Manager/KLDebugImGuiFeatureManager.h"

FKLDebugImGuiFeatureManagerEntryBase::FKLDebugImGuiFeatureManagerEntryBase(const size_t _ClassSize, const EFeatureEntryType _EntryType)
    : mEntryType(_EntryType)
{
    FKLDebugImGuiFeatureManager& WindowManager = FKLDebugImGuiFeatureManager::Get();
    WindowManager.AddEntry(_ClassSize, * this);
}
