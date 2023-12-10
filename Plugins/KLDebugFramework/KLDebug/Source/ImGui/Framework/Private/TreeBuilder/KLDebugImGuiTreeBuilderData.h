// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/KLDebugImGuiFeatureTypes.h"

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class KLDEBUGIMGUIFRAMEWORK_API FKLDebugImGuiTreeBuilderData : public FNoncopyable
{
public:
    explicit FKLDebugImGuiTreeBuilderData(const KL::Debug::ImGui::Features::Types::FeatureIndex _FeatureDataIndex, const FName& _FeatureImGuiPath);
    virtual ~FKLDebugImGuiTreeBuilderData() = default;

    UE_NODISCARD virtual const TArray<FName>& GetPathTokens() const = 0;

    UE_NODISCARD const FName& GetFullPath() const;
    UE_NODISCARD KL::Debug::ImGui::Features::Types::FeatureIndex GetFeatureDataIndex() const;

private:
    FName mTreePath;
    KL::Debug::ImGui::Features::Types::FeatureIndex mFeatureDataIndex = 0;
};

inline const FName& FKLDebugImGuiTreeBuilderData::GetFullPath() const
{
    return mTreePath;
}

inline KL::Debug::ImGui::Features::Types::FeatureIndex FKLDebugImGuiTreeBuilderData::GetFeatureDataIndex() const
{
    return mFeatureDataIndex;
}