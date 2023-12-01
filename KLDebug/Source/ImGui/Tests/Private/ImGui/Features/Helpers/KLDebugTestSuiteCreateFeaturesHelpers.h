// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// imgui core module
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"

class FKLDebugImGuiFeatureData;

namespace KL::Debug::TestSuite::Feature::Helpers
{
    void GetFeatures(TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& _FeaturePool, TArray<FKLDebugImGuiFeatureData>& _FeatureData);
}