#include "Message/Container/KLDebugImGuiNetworkingMessageContainer.h"

void FKLDebugImGuiNetworkingMessageContainer::Serialize(FArchive& _Archive)
{
    mMessageHeader.Serialize(_Archive);

    
}
