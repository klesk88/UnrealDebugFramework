// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// modules
#include "User/Framework/Public/Window/BottomBar/KLDebugBottomBarInterface.h"

class KLDEBUGEXAMPLES_API FKLDebugExampleBottomBar final : public IKLDebugBottomBarInterface
{
    KL_DEBUG_DERIVED_BOTTOMBAR(FKLDebugExampleBottomBar, IKLDebugBottomBarInterface)

public:
    // IKLDebugBottomBarInterface
    const FString& GetFriendlyName() const final;
    void Draw(const FKLDebugBottomBarDrawInput& _Input) const final;
    // IKLDebugBottomBarInterface
};
