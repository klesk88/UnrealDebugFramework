#pragma once

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"

namespace KL::Debug::ImGui::Features::Types
{
    typedef uint16 FeatureIndex;
    typedef uint16 FeatureOffset;
    typedef uint8  FeaturePoolValue;

    typedef uint16 FilterIndex;
    typedef uint8  FilterPoolValue;

    static constexpr FeatureIndex InvalidFeatureIndex = TNumericLimits<FeatureIndex>::Max();
}  // namespace KL::Debug::ImGui::Features::Types