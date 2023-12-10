// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/KLDebugNetworkingMessageInterface.h"

#include "Message/KLDebugNetworkingMessageSerializeInput.h"

// engine
#include "Serialization/Archive.h"

void IKLDebugNetworkingMessageInterface::Serialize(FArchive& _Archive)
{
    uint8 MessageVersion = GetMessageVersion();
    _Archive << MessageVersion;

    const FKLDebugNetworkingMessageSerializeInput Input{ MessageVersion, _Archive };
    SerializeChild(Input);
}
