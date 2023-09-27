#include "Message/KLDebugImGuiNetworkingMessageBase.h"

void FKLDebugImGuiNetworkingMessageBase::Write(const UWorld& _World, FArchive& _BitWriter)
{
    EKLDebugNetworkMessageTypes MessageType = GetMessageType();
    _BitWriter << MessageType;

    WriteChild(_World, _BitWriter);
}

void FKLDebugImGuiNetworkingMessageBase::Read(const UWorld& _World, FArchive& _BitReader)
{
    ReadChild(_World, _BitReader);
}
