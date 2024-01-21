// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Container/Manager/KLDebugImGuiFeaturesTypesContainerManager.h"
#include "Context/KLDebugContextInterface.h"
#include "Feature/KLDebugImGuiFeatureTypes.h"

// modules
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceBase.h"
#include "User/Framework/Internal/Feature/Interface/KLDebugFeatureInterfaceTypes.h"

// engine
#include "Templates/UnrealTemplate.h"
#include "UObject/Object.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiGatherFeatureInput final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiGatherFeatureInput(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex, const EImGuiInterfaceType _ContainerType, const UObject* _Object, const FKLDebugImGuiFeaturesTypesContainerManager& _ContainerManager);

    void SetFeatureData(IKLDebugFeatureInterfaceBase* _FeatureInterface, IKLDebugContextInterface* _FeatureContext);

    UE_NODISCARD const FKLDebugImGuiFeaturesTypesContainerManager& GetContainerManager() const;
    UE_NODISCARD const UObject* TryGetObject() const;
    UE_NODISCARD EImGuiInterfaceType GetContainerType() const;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureIndex() const;
    UE_NODISCARD IKLDebugFeatureInterfaceBase* TryGetFeatureInterface() const;
    UE_NODISCARD IKLDebugContextInterface* TryGetFeatureContext() const;

private:
    const FKLDebugImGuiFeaturesTypesContainerManager& mFeaturesContainerManager;
    const UObject* mObject = nullptr;
    EImGuiInterfaceType mContainerType = EImGuiInterfaceType::COUNT;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeatureIndex = KL::Debug::ImGui::Features::Types::InvalidFeatureIndex;

    IKLDebugFeatureInterfaceBase* mInterfaceBase = nullptr;
    IKLDebugContextInterface* mFeatureContext = nullptr;
};

inline void FKLDebugImGuiGatherFeatureInput::SetFeatureData(IKLDebugFeatureInterfaceBase* _FeatureInterface, IKLDebugContextInterface* _FeatureContext)
{
    mInterfaceBase = _FeatureInterface;
    mFeatureContext = _FeatureContext;
}

inline const FKLDebugImGuiFeaturesTypesContainerManager& FKLDebugImGuiGatherFeatureInput::GetContainerManager() const
{
    return mFeaturesContainerManager;
}

inline const UObject* FKLDebugImGuiGatherFeatureInput::TryGetObject() const
{
    return mObject;
}

inline EImGuiInterfaceType FKLDebugImGuiGatherFeatureInput::GetContainerType() const
{
    return mContainerType;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiGatherFeatureInput::GetFeatureIndex() const
{
    return mFeatureIndex;
}

inline IKLDebugFeatureInterfaceBase* FKLDebugImGuiGatherFeatureInput::TryGetFeatureInterface() const
{
    return mInterfaceBase;
}

inline IKLDebugContextInterface* FKLDebugImGuiGatherFeatureInput::TryGetFeatureContext() const
{
    return mFeatureContext;
}
