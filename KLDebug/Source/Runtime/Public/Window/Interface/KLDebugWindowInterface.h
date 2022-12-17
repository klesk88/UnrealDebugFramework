#pragma once

#include "Window/Interface/KLDebugWindowInterfaceMacros.h"

// engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
* Base class for any debug window to extend.
* IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_WINDOW() so that the class
* can auto subscribe to the system. For example:
* 
* class FClassDebug final : public IKLDebugWindow
* {
*   ...
* };
* 
* KL_DEBUG_CREATE_WINDOW(FClassDebug)
*/
class KLDEBUGRUNTIME_API IKLDebugWindow
{
public:
    virtual ~IKLDebugWindow() = default;
    
    //this is called once at game startup from the engine module to initialize the class
    virtual void Initialize();
};

inline void IKLDebugWindow::Initialize()
{
}
