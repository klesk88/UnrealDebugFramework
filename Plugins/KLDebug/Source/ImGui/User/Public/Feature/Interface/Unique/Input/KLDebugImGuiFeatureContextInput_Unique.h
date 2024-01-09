// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Feature/Interface/Context/KLDebugImGuiFeatureContextInput_Base.h"

class UWorld;

/*
 */
class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeatureContextInput_Unique final : public FKLDebugImGuiFeatureContextInput_Base
{
public:
    explicit FKLDebugImGuiFeatureContextInput_Unique(const ENetMode _CurrentNetMode, const UWorld& _World);

    UE_NODISCARD const UWorld& GetWorld() const;

private:
    const UWorld& mWorld;
};

inline const UWorld& FKLDebugImGuiFeatureContextInput_Unique::GetWorld() const
{
    return mWorld;
}
