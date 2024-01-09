// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Features/KLDebugImGuiServerUniqueFeatureData.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"

class FKLDebugImGuiFeatureContextInput_Unique;
class IKLDebugImGuiFeatureInterfaceBase;

class KLDEBUGIMGUISERVER_API FKLDebugImGuiServerUniqueFeatures final : public FNoncopyable
{
public:
    UE_NODISCARD const TArray<FKLDebugImGuiServerUniqueFeatureData>& GetFeatures() const;
    UE_NODISCARD TArray<FKLDebugImGuiServerUniqueFeatureData>& GetFeaturesMutable();
    void AddFeature(const FKLDebugImGuiFeatureContextInput_Unique& _Input, const IKLDebugImGuiFeatureInterfaceBase& _FeatureInterface, const KL::Debug::ImGui::Features::Types::FeatureIndex _ClientFeature, const KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeature);
    void RemoveFeature(KL::Debug::ImGui::Features::Types::FeatureIndex _ServerFeature);
    void Clear();

private:
    TArray<FKLDebugImGuiServerUniqueFeatureData> mFeatures;
};

inline const TArray<FKLDebugImGuiServerUniqueFeatureData>& FKLDebugImGuiServerUniqueFeatures::GetFeatures() const
{
    return mFeatures;
}

inline TArray<FKLDebugImGuiServerUniqueFeatureData>& FKLDebugImGuiServerUniqueFeatures::GetFeaturesMutable()
{
    return mFeatures;
}

inline void FKLDebugImGuiServerUniqueFeatures::Clear()
{
    mFeatures.Reset();
}