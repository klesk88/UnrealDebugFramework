#pragma once

#include "TreeBuilder/KLDebugImGuiTreeBuilderNode.h"

// engine
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"
#include "Misc/Optional.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGIMGUI_API FKLDebugImGuiFilterTreeNode final : public FKLDebugImGuiTreeBuilderNode
{
public:
    explicit FKLDebugImGuiFilterTreeNode(const KL::Debug::ImGui::Features::Types::FilterIndex _FilterIndex);
    explicit FKLDebugImGuiFilterTreeNode(const int32 _NodeDataIndex, const KL::Debug::ImGui::Features::Types::FilterIndex _FilterIndex);

    UE_NODISCARD uint16 GetFilterIndex() const;

private:
    KL::Debug::ImGui::Features::Types::FilterIndex mFilterIndex = TNumericLimits<KL::Debug::ImGui::Features::Types::FilterIndex>::Max();
};

inline uint16 FKLDebugImGuiFilterTreeNode::GetFilterIndex() const
{
    return mFilterIndex;
}
