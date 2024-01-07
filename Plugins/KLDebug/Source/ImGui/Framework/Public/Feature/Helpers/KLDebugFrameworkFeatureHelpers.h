#pragma once

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatformProcess.h"

class IKLDebugImGuiFeatureInterfaceBase;
class UWorld;

namespace KL::Debug::Feature::Helpers
{
    void OnFeatureUnselected(const UWorld& _World, UObject* _OwnerObject, IKLDebugImGuiFeatureInterfaceBase& _RemovedFeature);

}    // namespace KL::Debug::Feature::Helpers