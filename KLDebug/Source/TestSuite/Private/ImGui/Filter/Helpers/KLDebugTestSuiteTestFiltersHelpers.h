#pragma once

// imgui module
#include "ImGui/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"

namespace KL::Debug::TestSuite::Filter::Helpers
{
    void GetFilters(TArray<KL::Debug::ImGui::Features::Types::FilterPoolValue>& _FilterPool, TArray<KL::Debug::ImGui::Features::Types::FilterIndex>& _FilterOffsets);
}