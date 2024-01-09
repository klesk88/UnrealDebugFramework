// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/KLDebugImGuiServerUniqueFeatureData.h"

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Interface/Unique/KLDebugImGuiFeatureInterface_Unique.h"

FKLDebugImGuiServerUniqueFeatureData::FKLDebugImGuiServerUniqueFeatureData(const FKLDebugImGuiFeatureContextInput_Unique& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex)
    : mServerFeatureIndex(_ServerFeatureIndex)
    , mClientFeatureIndex(_ClientFeatureIndex)
{
    const IKLDebugImGuiFeatureInterface_Unique& UniqueFeature = static_cast<const IKLDebugImGuiFeatureInterface_Unique&>(_FeatureInterface);
    mContext = UniqueFeature.GetFeatureContext(_Input);
}
