// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Filter/Manager/KLDebugFilterManagerEntryBase.h"
#include "Internal/StaticMemory/KLDebugUtilsStaticMemoryManager.h"

// engine
#include "Containers/StaticArray.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugFilterManagerEntryBase;

class KLDEBUGUSERFRAMEWORK_API FKLDebugFilterManager final : public TKLDebugUtilsStaticMemoryManager<FKLDebugFilterManager, FKLDebugFilterManagerEntryBase>
{
public:
    FKLDebugFilterManager();
};
