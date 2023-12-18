// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Filter/Interface/KLDebugImGuiFilterInterfaceMacros.h"

// engine
#include "CoreMinimal.h"
#include "UObject/NameTypes.h"

class UObject;

/*
 * Base class for any debug window to extend.
 * IMPORTANT: in the .cpp file please use the macro KL_DEBUG_CREATE_FILTER() so that the class
 * can auto subscribe to the system. For example:
 *
 * class FClassFilter final : public IKLDebugImGuiFilterInterface
 * {
 *   ...
 * };
 *
 * FClassFilter.cpp
 *
 * KL_DEBUG_CREATE_FILTER(FClassFilter)
 */
class KLDEBUGIMGUIUSER_API IKLDebugImGuiFilterInterface
{
public:
    virtual ~IKLDebugImGuiFilterInterface();

    UE_NODISCARD virtual bool IsObjectValid(const UObject& _Object) const = 0;
    UE_NODISCARD virtual const FName& GetFilterID() const = 0;
};
