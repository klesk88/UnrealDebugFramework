// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Window/Input/KLDebugNetworkingWindowInputBase.h"

/*
 */
class KLDEBUGUSERNETWORKING_API FKLDebugNetworkingWindowServerTickInput final : public FKLDebugNetworkingWindowInputBase
{
public:
    explicit FKLDebugNetworkingWindowServerTickInput(const UWorld& _World, IKLDebugContextInterface* _ContextData, FArchive& _Archive);
};
