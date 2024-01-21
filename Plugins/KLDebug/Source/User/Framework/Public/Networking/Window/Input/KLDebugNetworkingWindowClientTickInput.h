// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Networking/Window/Input/KLDebugNetworkingWindowInputBase.h"

// engine
#include "Serialization/Archive.h"

/*
 * _Archive here is a writer
 */
class KLDEBUGUSERFRAMEWORK_API FKLDebugNetworkingWindowClientTickInput final : public FKLDebugNetworkingWindowInputBase
{
public:
    explicit FKLDebugNetworkingWindowClientTickInput(const UWorld& _World, IKLDebugContextInterface* _Context, FArchive& _Archive);
};
