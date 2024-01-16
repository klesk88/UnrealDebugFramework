// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Window/Toogle/KLDebugNetworkingMessage_WindowToogle.h"

FKLDebugNetworkingMessage_WindowToogle::FKLDebugNetworkingMessage_WindowToogle(const uint16 _NewSelection, const FName& _ID, const EKLDebugWindowTypes _WindowType)
    : TKLDebugNetworkingMessage_WindowBase<EKLDebugImGuiNetworkMessage::Client_WindowToogle>(_NewSelection, _ID, _WindowType)
    , mHasElement(true)
{
}

FKLDebugNetworkingMessage_WindowToogle::FKLDebugNetworkingMessage_WindowToogle(const EKLDebugWindowTypes _WindowType)
{
    mWindowType = _WindowType;
}

void FKLDebugNetworkingMessage_WindowToogle::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    // skip the parent call on purpose here as we deal us with the serialization

    FArchive& Archive = _Input.GetArchive();

    Archive << mHasElement;
    Archive << mWindowType;

    if (mHasElement)
    {
        Archive << mIndex;
        Archive << mID;
    }
}
