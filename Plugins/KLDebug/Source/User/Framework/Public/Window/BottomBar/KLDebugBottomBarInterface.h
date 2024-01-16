// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Context/User/KLDebugContextUserInterface.h"
#include "Internal/Networking/KLDebugNetworkCheckerInterface.h"
#include "Window/BottomBar/KLDebugBottomBarInterfaceMacros.h"

// modules
#include "Utils/Public/RTTI/KLDebugRTTIInterface.h"

// engine
#include "Containers/UnrealString.h"
#include "Templates/UnrealTypeTraits.h"
#include "UObject/NameTypes.h"

// c++
#include <type_traits>

class FKLDebugBottomBarDrawInput;
class UWorld;

#define KL_DEBUG_DERIVED_BOTTOMBAR(ItemType, ParentItemType) \
    KL_DEBUG_RTTI(ItemType, ParentItemType)                  \
    KL_DEBUG_NETWORK_CHECKER(ItemType, ParentItemType)

/*
 * Base class for any bottom bar to extend
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_BOTTOMBAR() in the .cpp so that the class
 * can auto subscribe to the system and the macro KL_DEBUG_DERIVED_BOTTOMBAR in the .h to implement the common methods. For example:
 *
 * class FClassBottomBar final : public IKLDebugBottomBarInterfaceBase
 * {
 *   ...KL_DEBUG_DERIVED_BOTTOMBAR(FClassBottomBar, IKLDebugBottomBarInterfaceBase)
 * };
 *
 * FClassBottomBar.cpp
 *
 * KL_DEBUG_CREATE_BOTTOMBAR(FClassBottomBar)
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugBottomBarInterface : public IKLDebugRTTIInterface, public IKLDebugContextUserInterface, public IKLDebugNetworkCheckerInterface
{
public:
    virtual ~IKLDebugBottomBarInterface();

    UE_NODISCARD virtual const FString& GetFriendlyName() const = 0;

    virtual void Draw(const FKLDebugBottomBarDrawInput& _Input) const = 0;

    virtual void OnSelect(const UWorld& _World);
    virtual void OnUnselect(const UWorld& _World);
};

inline void IKLDebugBottomBarInterface::OnSelect([[maybe_unused]] const UWorld& _World)
{
}

inline void IKLDebugBottomBarInterface::OnUnselect([[maybe_unused]] const UWorld& _World)
{
}
