// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Networking/Window/Input/KLDebugNetworkingWindowInputBase.h"

FKLDebugNetworkingWindowInputBase::FKLDebugNetworkingWindowInputBase(const UWorld& _World, IKLDebugContextInterface* _Context, FArchive& _Archive)
    : mWorld(_World)
    , mArchive(_Archive)
    , mContextData(_Context)
{
}
