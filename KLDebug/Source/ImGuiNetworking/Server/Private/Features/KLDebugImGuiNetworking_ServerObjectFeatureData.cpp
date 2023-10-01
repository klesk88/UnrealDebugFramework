#include "Features/KLDebugImGuiNetworking_ServerObjectFeatureData.h"

//modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"


FKLDebugImGuiNetworking_ServerObjectFeatureData::FKLDebugImGuiNetworking_ServerObjectFeatureData(const FKLDebugImGuiFeatureContextInput& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
    : mFeature(_FeatureIndex)
{
    mContext = _FeatureInterface.GetFeatureContext(_Input);
}

FKLDebugImGuiNetworking_ServerObjectFeatureData::~FKLDebugImGuiNetworking_ServerObjectFeatureData()
{
}

FKLDebugImGuiFeatureContext_Base* FKLDebugImGuiNetworking_ServerObjectFeatureData::GetContextMutable() const
{
    return mContext.Get();
}
