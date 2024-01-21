// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Feature/Interface/Input/KLDebugFeatureTickInput_Base.h"

FKLDebugFeatureTickInput_Base::FKLDebugFeatureTickInput_Base(const UWorld& _World, IKLDebugContextInterface* _Context)
    : mWorld(_World)
    , mContext(_Context)
{
}
