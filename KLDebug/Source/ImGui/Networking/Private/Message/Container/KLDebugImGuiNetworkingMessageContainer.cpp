// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Container/KLDebugImGuiNetworkingMessageContainer.h"

void FKLDebugImGuiNetworkingMessageContainer::Serialize(FArchive& _Archive)
{
    mMessageHeader.Serialize(_Archive);
}
