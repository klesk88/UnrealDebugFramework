// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Context/User/KLDebugContextUserInterface.h"
#include "Internal/Networking/KLDebugNetworkingGetterInterface.h"
#include "Internal/RTTI/KLDebugRTTIInterface.h"
#include "Mode/KLDebugModeInterfaceMacros.h"

// engine
#include "Containers/UnrealString.h"
#include "Templates/UnrealTypeTraits.h"
#include "UObject/NameTypes.h"

// c++
#include <type_traits>

class FKLDebugModeDrawCanvasInput;
class FKLDebugModeDrawImGuiInput;
class UWorld;

#define KL_DEBUG_DERIVED_MODE(ItemType, ParentItemType)                                                        \
    KL_DEBUG_RTTI(ItemType, ParentItemType)                                                                    \
    KL_DEBUG_NETWORK_GETTER_INTERFACE(ItemType)                                                                \
                                                                                                               \
    UE_NODISCARD inline bool RequiresDrawCanvas() const override                                               \
    {                                                                                                          \
        return !std::is_same_v<decltype(&IKLDebugModeInterface::DrawCanvas), decltype(&ItemType::DrawCanvas)>; \
    }                                                                                                          \
                                                                                                               \
private:

/*
 * Base class for any bottom bar to extend
 * IMPORTANT: after your class definition please use the macro KL_DEBUG_CREATE_MODE() in the .cpp so that the class
 * can auto subscribe to the system and the macro KL_DEBUG_DERIVED_MODE in the .h to implement the common methods. For example:
 *
 * class FClassMode final : public IKLDebugModeInterface
 * {
 *   ...KL_DEBUG_DERIVED_MODE(FClassMode, IKLDebugModeInterface)
 * };
 *
 * FClassMode.cpp
 *
 * KL_DEBUG_CREATE_MODE(FClassBottomBar)
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugModeInterface : public IKLDebugRTTIInterface, public IKLDebugContextUserInterface, public IKLDebugNetworkingGetterInterface
{
public:
    virtual ~IKLDebugModeInterface();

    UE_NODISCARD virtual const FString& GetFriendlyName() const = 0;

    virtual void DrawImGui(const FKLDebugModeDrawImGuiInput& _Input);
    virtual void DrawCanvas(FKLDebugModeDrawCanvasInput& _Input) const;

    virtual void OnSelect(const UWorld& _World);
    virtual void OnUnselect(const UWorld& _World);

    // implemented by KL_DEBUG_DERIVED_MODE macro
    UE_NODISCARD virtual bool RequiresDrawCanvas() const;
    // KL_DEBUG_DERIVED_MODE
};

inline void IKLDebugModeInterface::OnSelect([[maybe_unused]] const UWorld& _World)
{
}

inline void IKLDebugModeInterface::OnUnselect([[maybe_unused]] const UWorld& _World)
{
}

inline void IKLDebugModeInterface::DrawImGui([[maybe_unused]] const FKLDebugModeDrawImGuiInput& _Input)
{
}

inline void IKLDebugModeInterface::DrawCanvas([[maybe_unused]] FKLDebugModeDrawCanvasInput& _Input) const
{
    checkNoEntry();
}

inline bool IKLDebugModeInterface::RequiresDrawCanvas() const
{
    return false;
}