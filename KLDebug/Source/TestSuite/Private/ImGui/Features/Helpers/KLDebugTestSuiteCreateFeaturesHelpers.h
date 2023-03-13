#pragma once

// imgui module
#include "ImGui/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"

class FKLDebugImGuiFeatureData;

namespace KL::Debug::TestSuite::Feature::Helpers
{
    void GetFeatures(TArray<KL::Debug::ImGui::Features::Types::FeaturePoolValue>& _FeaturePool, TArray<FKLDebugImGuiFeatureData>& _FeatureData);
}