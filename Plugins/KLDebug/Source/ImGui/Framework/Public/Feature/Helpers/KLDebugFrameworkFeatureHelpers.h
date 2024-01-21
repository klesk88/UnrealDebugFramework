#pragma once

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatformProcess.h"

class IKLDebugFeatureInterfaceBase;
class UWorld;

namespace KL::Debug::Feature::Helpers
{
    void OnFeatureUnselected(const UWorld& _World, UObject* _OwnerObject, IKLDebugFeatureInterfaceBase& _RemovedFeature);

}    // namespace KL::Debug::Feature::Helpers