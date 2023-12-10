// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Filter/Manager/KLDebugImGuiFilterManagerEntryBase.h"
#include "StaticMemory/KLDebugImGuiStaticMemoryManager.h"

// engine
#include "Containers/StaticArray.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFilterManagerEntryBase;

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFilterManager final : public TKLDebugImGuiStaticMemoryManager<FKLDebugImGuiFilterManager, FKLDebugImGuiFilterManagerEntryBase>
{
public:
    FKLDebugImGuiFilterManager();
};
