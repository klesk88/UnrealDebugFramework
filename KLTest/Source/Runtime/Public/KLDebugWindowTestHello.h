#pragma once

#if WITH_KL_DEBUGGER

#include "ImGui/Public/Feature/Interface/KLDebugImGuiFeatureInterface.h"

// engine
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"

class FKLDebugWindowTest12 final : public IKLDebugImGuiFeatureInterface
{
public:
    void Initialize()
    {
        TestVAlue = true;
    }

    void Update()
    {
    }

    
    const FGameplayTag& GetTag() const;

private:
    bool TestVAlue = false;
};

class FKLDebugWindowTest final : public IKLDebugImGuiFeatureInterface
{
public:
    FKLDebugWindowTest()
    {
        mString = TEXT("hello");
    }

    void Initialize()
    {
        mString = TEXT("hola");
    }

    void Update()
    {
    }

    const FGameplayTag& GetTag() const;

private:
    float mTest = 0.f;
    FString mString;

};


KL_DEBUG_CREATE_WINDOW(FKLDebugWindowTest, EFeatureEntryType::Default)
KL_DEBUG_CREATE_WINDOW(FKLDebugWindowTest12, EFeatureEntryType::Default)

#endif