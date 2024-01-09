// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Templates/UniquePtr.h"

class FDebugRenderSceneProxy;

struct FDebugDrawDelegateHelper;

namespace KL::Debug::Framework::Rendering
{
    using GatherSceneProxyCallback = TFunctionRef<void(TUniquePtr<FDebugRenderSceneProxy>&& /*_SceneProxy*/, TUniquePtr<FDebugDrawDelegateHelper>&& /*_DrawDelegateHelper*/)>;
}