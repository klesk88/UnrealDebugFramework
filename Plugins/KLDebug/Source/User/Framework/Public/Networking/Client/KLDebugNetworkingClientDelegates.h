// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"

DECLARE_DELEGATE_RetVal(uint32, FGetDebugServerSocketPort);

namespace KL::Debug::Client::Delegates
{
    KLDEBUGUSERFRAMEWORK_API void BindGetDebugServerSocketPort(const FGetDebugServerSocketPort& _Delegate);
    KLDEBUGUSERFRAMEWORK_API void UnbindGetDebugServerSocketPort();

}    // namespace KL::Debug::Client::Delegates