#include "KLDebugWindowTestHello.h"

#if WITH_KL_DEBUGGER

// engine
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TESTDebugGf, "KL.Debug.ImGui.AI.Testf");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TESTDebugSf, "KL.Debug.ImGui.Physics.TestfSecond");

const FGameplayTag& FKLDebugWindowTest12::GetTag() const
{
    static FGameplayTag TAg = TESTDebugGf.GetTag();
    return TAg;
}

const FGameplayTag& FKLDebugWindowTest34::GetTag() const
{
    static FGameplayTag TAg = TESTDebugSf.GetTag();
    return TAg;
}

#endif