#include "Features/KLDebugImGuiServerObjectFeatureData.h"

//modules
#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"
#include "ImGui/User/Public/Feature/Interface/Context/KLDebugImGuiFeatureContext_Base.h"
#include "ImGui/User/Public/Feature/Networking/KLDebugImGuiFeature_NetworkingInterface.h"


FKLDebugImGuiServerObjectFeatureData::FKLDebugImGuiServerObjectFeatureData(const FKLDebugImGuiFeatureContextInput& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex)
    : mFeature(_FeatureIndex)
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
