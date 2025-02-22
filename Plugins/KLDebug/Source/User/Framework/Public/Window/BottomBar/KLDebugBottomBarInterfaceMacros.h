// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/BottomBar/StaticMemory/KLDebugBottomBarManagerEntry.h"

// engine
#include "CoreMinimal.h"

/* macro which allow us to statically allocate a variable of the type wanted
 * so that we can auto register to the system.
 * NOTE: This ensures that we create also one class ever of the type passed (otherwise the compiler
 * will output an error for multiple class definitions)
 */

#define KL_DEBUG_CREATE_BOTTOMBAR(_Class)                                                        \
    namespace KL::Debug::BottomBar                                                               \
    {                                                                                            \
        static inline const TKLDebugBottomBarManagerEntry<_Class> Instanced_##_Class{ #_Class }; \
    }
