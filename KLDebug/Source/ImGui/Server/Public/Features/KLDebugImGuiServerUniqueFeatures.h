#pragma once

//modules
#include "ImGui/Framework/Public/Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerUniqueFeatures final : public FNoncopyable
{
public:
    UE_NODISCARD const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& GetFeatures() const;
    void AddFeature(const KL::Debug::ImGui::Features::Types::FeatureIndex _Feature);
    void RemoveFeature(KL::Debug::ImGui::Features::Types::FeatureIndex _Feature);
    void Clear();

private:
    TArray<KL::Debug::ImGui::Features::Types::FeatureIndex> mFeatures;
};

inline const TArray<KL::Debug::ImGui::Features::Types::FeatureIndex>& FKLDebugImGuiServerUniqueFeatures::GetFeatures() const
{
    return mFeatures;
}

inline void FKLDebugImGuiServerUniqueFeatures::Clear()
{
    mFeatures.Reset();
}