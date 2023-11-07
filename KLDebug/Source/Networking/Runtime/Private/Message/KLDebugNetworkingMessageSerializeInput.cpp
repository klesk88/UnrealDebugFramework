#include "Message/KLDebugNetworkingMessageSerializeInput.h"

FKLDebugNetworkingMessageSerializeInput::FKLDebugNetworkingMessageSerializeInput(const uint8 _MessageVersion, FArchive& _Archive)
    : mArchive(_Archive)
    , mMessageVersion(_MessageVersion)
{
}
