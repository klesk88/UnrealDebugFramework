#include "Features/KLDebugImGuiServerUniqueFeatureData.h"

//modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"

FKLDebugImGuiServerUniqueFeatureData::FKLDebugImGuiServerUniqueFeatureData(const FKLDebugImGuiFeatureContextInput& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex)
    : mServerFeatureIndex(_ServerFeatureIndex)
    , mClientFeatureIndex(_ClientFeatureIndex)
{
    mContext = _FeatureInterface.GetFeatureContext(_Input);
}