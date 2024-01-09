// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/StaticBitArray.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"

namespace KL::Debug::ImGui::Features::Types
{
    typedef uint16 FeatureIndex;
    typedef uint16 FeatureOffset;
    typedef uint8 FeaturePoolValue;

    typedef uint16 FilterIndex;
    typedef uint8 FilterPoolValue;

    static constexpr FeatureIndex InvalidFeatureIndex = TNumericLimits<FeatureIndex>::Max();

    enum class EFeatureEnableType : uint8
    {
        Canvas = 0,
        SceneProxy,
        UpdateSceneProxy,
        Tick,
        Render,

        Count
    };

    using FeatureEnableSet = TStaticBitArray<static_cast<uint32>(EFeatureEnableType::Count)>;

    inline void UpdateFeatureEnableSet(const EFeatureEnableType& _Type, const bool _ShouldUpdate, FeatureEnableSet& _SystemBitset)
    {
        _SystemBitset[static_cast<int32>(_Type)] = _SystemBitset[static_cast<int32>(_Type)] || _ShouldUpdate;
    }
}    // namespace KL::Debug::ImGui::Features::Types