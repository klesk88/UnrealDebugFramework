// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Subsystem/Engine/KLDebugImGuiNetworkingGameThreadUpdateContextBase.h"

FKLDebugImGuiNetworkingGameThreadUpdateContextBase::~FKLDebugImGuiNetworkingGameThreadUpdateContextBase()
{
}

void FKLDebugImGuiNetworkingGameThreadUpdateContextBase::SetNewWorlds(const TArray<TWeakObjectPtr<const UWorld>>& _NewWorlds)
{
    mNewWorlds = _NewWorlds;
}
