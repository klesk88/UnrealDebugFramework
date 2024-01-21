// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Public/Window/KLDebugWindowTypes.h"

// engine
#include "Delegates/DelegateCombinations.h"

class FKLDebugImGuiFeatureStatusUpdateData;
class FKLDebugImGuiFeaturesTickInput;
class UKLDebugImGuiWorldSubsystem;
class IKLDebugNetworkingGetterInterface;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnImGuiFeatureStateUpdated, const FKLDebugImGuiFeatureStatusUpdateData& /*_FeatureUpdateData*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnImGuiWorldSubsystemChangeState, const bool /*_Added*/, UKLDebugImGuiWorldSubsystem& /*_ImGuiSubsystem*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFeaturesTick, const FKLDebugImGuiFeaturesTickInput& /*_Input*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShouldTickDelegate, bool& /*_ShouldTick*/);

DECLARE_MULTICAST_DELEGATE_FiveParams(FOnWindowStatusChange, const int32 _WindowIndex, const EKLDebugWindowTypes _WindowType, const FName& _WindowID, const IKLDebugNetworkingGetterInterface* _PrevNetworkInterfaceChecker, const IKLDebugNetworkingGetterInterface* _NewNetworkInterfaceChecker);

namespace KL::Debug::ImGui::Framework
{
    KLDEBUGIMGUIFRAMEWORK_API extern FOnImGuiWorldSubsystemChangeState OnImGuiWorldSusbsytemStateChange;
}    // namespace KL::Debug::ImGui::Framework