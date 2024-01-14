// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Public/Mode/KLDebugModeInterface.h"

class KLDEBUGEXAMPLES_API FKLDebugExampleMode final : public IKLDebugModeInterface
{
    KL_DEBUG_DERIVED_MODE(FKLDebugExampleMode, IKLDebugModeInterface)

public:
    // IKLDebugModeInterface
    const FString& GetFriendlyName() const;
    void DrawImGui(const FKLDebugModeDrawImGuiInput& _Input) final;
    void DrawCanvas(FKLDebugModeDrawCanvasInput& _Input) const final;
    // IKLDebugModeInterface
};
