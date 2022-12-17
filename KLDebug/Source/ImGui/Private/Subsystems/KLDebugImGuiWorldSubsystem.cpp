#include "Subsystems/KLDebugImGuiWorldSubsystem.h"

#include "Subsystems/KLDebugImGuiEngineSubsystem.h"

bool UKLDebugImGuiWorldSubsystem::ShouldCreateSubsystem(UObject* _Outer) const
{
    return Super::ShouldCreateSubsystem(_Outer);
}

void UKLDebugImGuiWorldSubsystem::Initialize(FSubsystemCollectionBase& _Collection)
{
    RegisterCallbacks();
}

void UKLDebugImGuiWorldSubsystem::RegisterCallbacks()
{
    //mSelectObjectEvent = USelection::SelectObjectEvent.AddUObject(this, &UKLDebugImGuiWorldSubsystem::OnObjectSelected);
}

void UKLDebugImGuiWorldSubsystem::Deinitialize()
{
    UnregisterCallbacks();
}

void UKLDebugImGuiWorldSubsystem::UnregisterCallbacks()
{
    //if (mSelectObjectEvent.IsValid())
    //{
    //    USelection::SelectObjectEvent.Remove(mSelectObjectEvent);
    //    mSelectObjectEvent.Reset();
    //}
}

void UKLDebugImGuiWorldSubsystem::OnObjectSelected(UObject* _Object)
{
    if (!_Object)
    {
        return;
    }


}
