// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class FString;
class IKLDebugFeatureInterfaceBase;

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiFeatureData final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeatureData(const KL::Debug::ImGui::Features::Types::FeatureOffset _FeatureOffset);

    void Init(const IKLDebugFeatureInterfaceBase& _FeatureInterface, TArray<FString>& _PathString);

    [[nodiscard]] const TArray<FName>& GetImGuiPathTokens() const;
    [[nodiscard]] KL::Debug::ImGui::Features::Types::FeatureOffset GetFeatureOffset() const;

private:
    TArray<FName> mImGuiPathTokens;
    // offset between features so that we can retrieve them correctly from the pool. Each entry is the
    // start address index of a new feature inside mFeaturesPool.
    KL::Debug::ImGui::Features::Types::FeatureOffset mFeaturesOffset;
};

inline const TArray<FName>& FKLDebugImGuiFeatureData::GetImGuiPathTokens() const
{
    return mImGuiPathTokens;
}

inline KL::Debug::ImGui::Features::Types::FeatureOffset FKLDebugImGuiFeatureData::GetFeatureOffset() const
{
    return mFeaturesOffset;
}