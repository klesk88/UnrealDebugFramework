// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Mode/Input/KLDebugModeInputBase.h"

FKLDebugModeInputBase::FKLDebugModeInputBase(const UWorld& _World, IKLDebugContextInterface* _Context)
    : mWorld(_World)
    , mContext(_Context)
{
}
