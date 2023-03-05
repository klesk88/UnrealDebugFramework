#pragma once

#include "Filter/Manager/KLDebugImGuiFilterManagerEntry.h"

// engine
#include "CoreMinimal.h"
#include "UObject/NameTypes.h"

/* macro which allow us to statically allocate a variable of the type wanted
 * so that we can auto register to the system.
 * NOTE: This ensures that we create also one class ever of the type passed (otherwise the compiler
 * will output an error for multiple class definitions)
 */
#define KL_DEBUG_CREATE_FILTER(_FilterClass)                                                        \
    namespace KL::Debug::ImGui::Filters                                                             \
    {                                                                                               \
        static inline const TKLDebugImGuiFilterManagerEntry<_FilterClass> Instanced_##_FilterClass; \
    }

#define KL_DEBUG_FILTER_GENERATE(_FilterName)                   \
public:                                                         \
    UE_NODISCARD inline static const FName& StaticGetFilterID() \
    {                                                           \
        static FName FilterID(TEXT(#_FilterName));              \
        return FilterID;                                        \
    }                                                           \
                                                                \
    UE_NODISCARD inline const FName& GetFilterID() const final  \
    {                                                           \
        return StaticGetFilterID();                             \
    }                                                           \
                                                                \
private:
