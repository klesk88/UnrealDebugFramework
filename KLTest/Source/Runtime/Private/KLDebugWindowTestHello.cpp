#include "KLDebugWindowTestHello.h"

#if WITH_KL_DEBUGGER

#include "ImGui/Public/Filter/Implementations/KLDebugImGuiFilterAI.h"
#include "ImGui/Public/Filter/Implementations/KLDebugImGuiFilterPlayer.h"

const FName& FKLDebugWindowTest12::GetImGuiPath() const
{
    static const FName WindowName(TEXT("WindowTest12"));
    return WindowName;
}

void FKLDebugWindowTest12::GetFilterPath(TArray<FName>& _OutFilters) const
{
    _OutFilters.Emplace(KLDebugImGuiFilterAI::StaticGetFilterID());
}

bool FKLDebugWindowTest12::DoesSupportObject(const UObject& _Object) const
{
    return true;
}

const FName& FKLDebugWindowTest34::GetImGuiPath() const
{
    static const FName WindowName(TEXT("WindowTest34"));
    return WindowName;
}

void FKLDebugWindowTest34::GetFilterPath(TArray<FName>& _OutFilters) const
{
    _OutFilters.Emplace(KLDebugImGuiFilterPlayer::StaticGetFilterID());
}

bool FKLDebugWindowTest34::DoesSupportObject(const UObject& _Object) const
{
    return true;
}

#endif