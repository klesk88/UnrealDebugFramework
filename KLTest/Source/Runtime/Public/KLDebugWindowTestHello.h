#pragma once

#include "ImGui/Public/Feature/Interface/KLDebugImGuiFeatureInterface.h"

//engine
#include "CoreMinimal.h"
#include "Containers/UnrealString.h"

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

private:
    float mTest = 0.f;
    FString mString;

};


KL_DEBUG_CREATE_WINDOW(FKLDebugWindowTest)
KL_DEBUG_CREATE_WINDOW(FKLDebugWindowTest12)
