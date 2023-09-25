#include "Message/KLDebugImGuiNetworkingMessageBase.h"

void FKLDebugImGuiNetworkingMessageBase::Write(const UWorld& _World, FBitWriter& _BitWriter)
{
    EKLDebugNetworkMessageTypes MessageType = GetMessageType();
    _BitWriter << MessageType;

    WriteChild(_World, _BitWriter);
}

void FKLDebugImGuiNetworkingMessageBase::Read(const UWorld& _World, FBitReader& _BitReader)
{
    ReadChild(_World, _BitReader);
}
