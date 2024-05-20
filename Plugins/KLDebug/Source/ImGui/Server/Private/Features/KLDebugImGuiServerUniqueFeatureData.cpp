// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/KLDebugImGuiServerUniqueFeatureData.h"

// modules
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceBase.h"
#include "User/Framework/Public/Feature/Interface/Unique/KLDebugFeatureInterface_Unique.h"

FKLDebugImGuiServerUniqueFeatureData::FKLDebugImGuiServerUniqueFeatureData(const FKLDebugContextGetterInput& _Input, const IKLDebugFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex)
    : mServerFeatureIndex(_ServerFeatureIndex)
    , mClientFeatureIndex(_ClientFeatureIndex)
{
    const IKLDebugFeatureInterface_Unique& UniqueFeature = static_cast<const IKLDebugFeatureInterface_Unique&>(_FeatureInterface);
    mContext = UniqueFeature.GetContext(_Input);
}

const IKLDebugContextInterface* FKLDebugImGuiServerUniqueFeatureData::GetContext() const
{
    return GetFeatureContextMutable();
}

IKLDebugContextInterface* FKLDebugImGuiServerUniqueFeatureData::GetFeatureContextMutable() const
{
    return mContext.Get();
}
