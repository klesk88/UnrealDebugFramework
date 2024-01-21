// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Feature/Manager/KLDebugFeatureManagerEntryBase.h"
#include "Internal/StaticMemory/KLDebugUtilsStaticMemoryManager.h"

class KLDEBUGUSERFRAMEWORK_API FKLDebugFeatureManager final : public TKLDebugUtilsStaticMemoryManager<FKLDebugFeatureManager, FKLDebugFeatureManagerEntryBase>
{
};
