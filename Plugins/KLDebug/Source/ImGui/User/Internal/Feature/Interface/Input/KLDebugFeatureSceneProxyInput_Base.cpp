// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Interface/Input/KLDebugFeatureSceneProxyInput_Base.h"

FKLDebugFeatureSceneProxyInput_Base::FKLDebugFeatureSceneProxyInput_Base(const UPrimitiveComponent& _RenderingComponent, FKLDebugImGuiFeatureContext_Base* _Context)
    : mRenderingComponent(_RenderingComponent)
    , mContext(_Context)
{
}
