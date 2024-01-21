// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Feature/Interface/Input/KLDebugFeatureImGuiInput_Base.h"

FKLDebugFeatureImGuiInput_Base::FKLDebugFeatureImGuiInput_Base(const UWorld& _World, bool& _IsWindowOpen, IKLDebugContextInterface* _FeatureContext)
    : mWorld(_World)
    , mIsWindowOpen(_IsWindowOpen)
    , mContextData(_FeatureContext)
{
}
