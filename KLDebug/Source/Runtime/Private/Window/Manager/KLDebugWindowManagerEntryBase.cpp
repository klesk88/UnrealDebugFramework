#include "Window/Manager/KLDebugWindowManagerEntryBase.h"
#include "Window/Manager/KLDebugWindowManager.h"

FKLDebugWindowManagerEntryBase::FKLDebugWindowManagerEntryBase()
{
    FKLDebugWindowManager& WindowManager = FKLDebugWindowManager::Get();
    WindowManager.AddEntry(*this);
}
