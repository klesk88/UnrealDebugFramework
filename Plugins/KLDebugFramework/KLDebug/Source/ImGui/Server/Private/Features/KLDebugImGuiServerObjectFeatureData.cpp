// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/KLDebugImGuiServerObjectFeatureData.h"

// modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"

FKLDebugImGuiServerObjectFeatureData::FKLDebugImGuiServerObjectFeatureData(const FKLDebugImGuiFeatureContextInput& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex)
    : mServerFeatureIndex(_ServerFeatureIndex)
    , mClientFeatureIndex(_ClientFeatureIndex)
{
    mContext = _FeatureInterface.GetFeatureContext(_Input);
}

FKLDebugImGuiServerObjectFeatureData::~FKLDebugImGuiServerObjectFeatureData()
{
}

FKLDebugImGuiFeatureContext_Base* FKLDebugImGuiServerObjectFeatureData::GetContextMutable() const
{
    return mContext.Get();
}
