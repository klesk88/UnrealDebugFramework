// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Filter/Interface/KLDebugFilterInterfaceMacros.h"

// engine
#include "CoreMinimal.h"
#include "UObject/NameTypes.h"

class UObject;

/*
 * Base class for any debug window to extend.
 * IMPORTANT: in the .cpp file please use the macro KL_DEBUG_CREATE_FILTER() so that the class
 * can auto subscribe to the system. For example:
 *
 * class FClassFilter final : public IKLDebugFilterInterface
 * {
 *   ...
 * };
 *
 * FClassFilter.cpp
 *
 * KL_DEBUG_CREATE_FILTER(FClassFilter)
 */
class KLDEBUGUSERFRAMEWORK_API IKLDebugFilterInterface
{
public:
    virtual ~IKLDebugFilterInterface();

    [[nodiscard]] virtual bool IsObjectValid(const UObject& _Object) const = 0;
    [[nodiscard]] virtual const FName& GetFilterID() const = 0;
};
