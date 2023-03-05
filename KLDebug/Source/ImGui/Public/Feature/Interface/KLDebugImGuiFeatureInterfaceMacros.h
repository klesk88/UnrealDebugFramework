#pragma once

#include "Feature/Manager/KLDebugImGuiFeatureManagerEntry.h"

// engine
#include "CoreMinimal.h"

/* macro which allow us to statically allocate a variable of the type wanted
 * so that we can auto register to the system.
 * _EntryType represents an entry of EFeatureEntryType
 * NOTE: This ensures that we create also one class ever of the type passed (otherwise the compiler
 * will output an error for multiple class definitions)
 */
#define KL_DEBUG_CREATE_WINDOW(_Class)                                                     \
    namespace KL::Debug::Windows                                                           \
    {                                                                                      \
        static inline const TKLDebugImGuiFeatureManagerEntry<_Class> Instanced_##_Class{}; \
    }
