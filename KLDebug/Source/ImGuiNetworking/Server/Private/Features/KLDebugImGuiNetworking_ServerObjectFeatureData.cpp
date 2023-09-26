#include "Features/KLDebugImGuiNetworking_ServerObjectFeatureData.h"

//imgui module
#include "ImGui/Public/Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "ImGui/Public/Feature/Interface/Private/KLDebugImGuiFeatureInterfaceBase.h"
//ImGuiNetworking module
#include "ImGuiNetworking/Runtime/Public/Interface/KLDebugImGuiNetworking_FeatureInterface.h"

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
