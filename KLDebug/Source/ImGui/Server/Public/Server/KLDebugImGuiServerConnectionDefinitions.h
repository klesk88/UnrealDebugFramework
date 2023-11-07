#pragma once

//engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"

namespace KL::Debug::ImGui::Networking::Server
{
    typedef uint16 CacheConnectionID;

    static constexpr CacheConnectionID InvalidCacheConnectionID = TNumericLimits<CacheConnectionID>::Max();

    static uint32 FeatureMaxMessageDataSize = 100;
    static float FeatureMaxMessageDataSizeInv = 1.f / static_cast<float>(FeatureMaxMessageDataSize);
} // KL::Debug::ImGui::Networking::Server