#pragma once

#include "Window/Manager/KLDebugWindowManagerEntryBase.h"

template<typename WindowInterfaceType>
class TKLDebugWindowManagerEntry final : public FKLDebugWindowManagerEntryBase
{
public:
    //FKLDebugWindowManagerEntryBase
    UE_NODISCARD IKLDebugWindow& GetInterface() final;
    //FKLDebugWindowManagerEntryBase

private:
    WindowInterfaceType mInterface;
};

template<typename WindowInterfaceType>
IKLDebugWindow& TKLDebugWindowManagerEntry<WindowInterfaceType>::GetInterface()
{
    return mInterface;
}
