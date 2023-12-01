// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// imgui core module
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"

namespace KL::Debug::TestSuite::Filter::Helpers
{
    void GetFilters(TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue>& _FilterPool, TArray<KL::Debug::ImGui::Features::Types::FilterIndex>& _FilterOffsets);
}