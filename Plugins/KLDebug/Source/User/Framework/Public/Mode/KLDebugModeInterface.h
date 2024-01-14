// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Context/User/KLDebugContextUserInterface.h"
#include "Mode/KLDebugModeInterfaceMacros.h"

// modules
#include "Utils/Public/RTTI/KLDebugRTTIInterface.h"

// engine
#include "Templates/UnrealTypeTraits.h"
#include "UObject/NameTypes.h"

class FKLDebugModeDrawCanvasInput;
class FKLDebugModeDrawImGuiInput;
class UWorld;

#define KL_DEBUG_DERIVED_MODE(ItemType, ParentItemType) \
    KL_DEBUG_RTTI(ItemType, ParentItemType)

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
class KLDEBUGUSERFRAMEWORK_API IKLDebugModeInterface : public IKLDebugRTTIInterface, public IKLDebugContextUserInterface
{
public:
    virtual ~IKLDebugModeInterface();

    virtual void DrawImGui(const FKLDebugModeDrawImGuiInput& _Input);
    virtual void DrawCanvas(const FKLDebugModeDrawCanvasInput& _Input);

    virtual void OnSelect(const UWorld& _World);
    virtual void OnUnselect(const UWorld& _World);
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

inline void IKLDebugModeInterface::DrawCanvas([[maybe_unused]] const FKLDebugModeDrawCanvasInput& _Input)
{
}
