// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Delegates/DelegateCombinations.h"

class FKLDebugImGuiFeatureStatusUpdateData;
class FKLDebugImGuiFeaturesTickInput;
class UKLDebugImGuiWorldSubsystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnImGuiFeatureStateUpdated, const FKLDebugImGuiFeatureStatusUpdateData& /*_FeatureUpdateData*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnImGuiWorldSubsystemChangeState, const bool /*_Added*/, UKLDebugImGuiWorldSubsystem& /*_ImGuiSubsystem*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFeaturesTick, const FKLDebugImGuiFeaturesTickInput& /*_Input*/);

namespace KL::Debug::ImGui::Framework
{
    KLDEBUGIMGUIFRAMEWORK_API extern FOnImGuiWorldSubsystemChangeState OnImGuiWorldSusbsytemStateChange;
}    // namespace KL::Debug::ImGui::Framework