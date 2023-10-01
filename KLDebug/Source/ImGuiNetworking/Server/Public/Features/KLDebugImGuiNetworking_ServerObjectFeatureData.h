#pragma once

//imgui core module
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeatureContext_Base;
class FKLDebugImGuiFeatureContextInput;
class IKLDebugImGuiFeatureInterfaceBase;


class KLDEBUGIMGUINETWORKINGSERVER_API FKLDebugImGuiNetworking_ServerObjectFeatureData final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiNetworking_ServerObjectFeatureData(const FKLDebugImGuiFeatureContextInput& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex);
    ~FKLDebugImGuiNetworking_ServerObjectFeatureData();
    UE_NODISCARD bool operator==(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const;

    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureIndex() const;
    UE_NODISCARD FKLDebugImGuiFeatureContext_Base* GetContextMutable() const;
    UE_NODISCARD const FKLDebugImGuiFeatureContext_Base* GetContext() const;

private:
    TUniquePtr<FKLDebugImGuiFeatureContext_Base> mContext;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeature;
};

inline bool FKLDebugImGuiNetworking_ServerObjectFeatureData::operator==(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const
{
    return mFeature == _FeatureIndex;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiNetworking_ServerObjectFeatureData::GetFeatureIndex() const
{
    return mFeature;
}

inline const FKLDebugImGuiFeatureContext_Base* FKLDebugImGuiNetworking_ServerObjectFeatureData::GetContext() const
{
    return GetContextMutable();
}