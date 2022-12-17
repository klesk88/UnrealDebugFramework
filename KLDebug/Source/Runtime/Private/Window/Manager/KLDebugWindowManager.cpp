#include "Window/Manager/KLDebugWindowManager.h"
#include "Window/Manager/KLDebugWindowManagerEntryBase.h"

void FKLDebugWindowManager::AddEntry(FKLDebugWindowManagerEntryBase& _NewEntry)
{
    ++mCount;

    if (!mHead)
    {
        mHead = &_NewEntry;
        mTail = &_NewEntry;
        return;
    }

    mTail->AddNextEntry(_NewEntry);
    mTail = &_NewEntry;
}
