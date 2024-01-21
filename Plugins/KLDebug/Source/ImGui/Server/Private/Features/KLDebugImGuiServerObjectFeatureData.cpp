// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Features/KLDebugImGuiServerObjectFeatureData.h"

// modules
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceBase.h"
#include "User/Framework/Public/Context/KLDebugContextInterface.h"
#include "User/Framework/Public/Feature/Interface/Selectable/KLDebugFeatureInterface_Selectable.h"

FKLDebugImGuiServerObjectFeatureData::FKLDebugImGuiServerObjectFeatureData(const FKLDebugContextGetterInput_Selectable& _Input, const IKLDebugFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeatureIndex, const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeatureIndex)
    : mServerFeatureIndex(_ServerFeatureIndex)
    , mClientFeatureIndex(_ClientFeatureIndex)
{
    const IKLDebugFeatureInterface_Selectable& SelectableFeature = static_cast<const IKLDebugFeatureInterface_Selectable&>(_FeatureInterface);
    mContext = SelectableFeature.GetContext(_Input);
}

FKLDebugImGuiServerObjectFeatureData::~FKLDebugImGuiServerObjectFeatureData()
{
}

IKLDebugContextInterface* FKLDebugImGuiServerObjectFeatureData::GetContextMutable() const
{
    return mContext.Get();
}
