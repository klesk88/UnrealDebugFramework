#pragma once

#include "Runtime/Public/Window/Interface/KLDebugWindowInterface.h"

//engine
#include "CoreMinimal.h"

class FKLDebugWindowTest12 final : public IKLDebugWindow
{
public:
};

class FKLDebugWindowTest final : public IKLDebugWindow
{
public:
};


KL_DEBUG_CREATE_WINDOW(FKLDebugWindowTest)
KL_DEBUG_CREATE_WINDOW(FKLDebugWindowTest12)
