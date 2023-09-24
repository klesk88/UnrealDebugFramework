#pragma once

//engine
#include "Delegates/DelegateCombinations.h"

class FKLDebugImGuiFeatureStatusUpdateData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnImGuiFeatureStateUpdated, const FKLDebugImGuiFeatureStatusUpdateData& /*_FeatureUpdateData*/)