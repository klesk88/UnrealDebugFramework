#include "Feature/Manager/KLDebugImGuiFeatureManager.h"

#include "Feature/Manager/KLDebugImGuiFeatureManagerEntryBase.h"

void FKLDebugImGuiFeatureManager::AddEntry(const size_t _ClassSize, FKLDebugImGuiFeatureManagerEntryBase& _NewEntry)
{
    ++mCount;
    mTotalBytesCount += _ClassSize;

    if (!mHead)
    {
        mHead = &_NewEntry;
        mTail = &_NewEntry;
        return;
    }

    mTail->AddNextEntry(_NewEntry);
    mTail = &_NewEntry;
}
