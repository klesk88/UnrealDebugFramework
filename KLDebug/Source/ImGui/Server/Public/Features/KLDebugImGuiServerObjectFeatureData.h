#pragma once

//modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UniquePtr.h"
#include "Templates/UnrealTemplate.h"

class FKLDebugImGuiFeatureContext_Base;
class FKLDebugImGuiFeatureContextInput;
class IKLDebugImGuiFeatureInterfaceBase;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerObjectFeatureData final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiServerObjectFeatureData(const FKLDebugImGuiFeatureContextInput& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex);
    ~FKLDebugImGuiServerObjectFeatureData();
    UE_NODISCARD bool operator==(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const;

    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureIndex() const;
    UE_NODISCARD FKLDebugImGuiFeatureContext_Base* GetContextMutable() const;
    UE_NODISCARD const FKLDebugImGuiFeatureContext_Base* GetContext() const;

private:
    TUniquePtr<FKLDebugImGuiFeatureContext_Base> mContext;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeature;
};

inline bool FKLDebugImGuiServerObjectFeatureData::operator==(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureIndex) const
{
    return mFeature == _FeatureIndex;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiServerObjectFeatureData::GetFeatureIndex() const
{
    return mFeature;
}

inline const FKLDebugImGuiFeatureContext_Base* FKLDebugImGuiServerObjectFeatureData::GetContext() const
{
    return GetContextMutable();
}