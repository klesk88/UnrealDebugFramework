// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/KLDebugImGuiNetworkingMessage_Base.h"

// modules
#include "Networking/Runtime/Public/Message/KLDebugNetworkingMessageSerializeInput.h"
#include "User/Framework/Public/Window/KLDebugWindowTypes.h"

// engine
#include "GenericPlatform/GenericPlatform.h"
#include "Math/NumericLimits.h"
#include "UObject/NameTypes.h"

class FKLDebugNetworkingMessageSerializeInput;

template <EKLDebugImGuiNetworkMessage MessageType>
class TKLDebugNetworkingMessage_WindowBase : public TKLDebugImGuiNetworkingMessage_Base<MessageType>
{
public:
    explicit TKLDebugNetworkingMessage_WindowBase() = default;
    explicit TKLDebugNetworkingMessage_WindowBase(const uint16 _NewSelection, const FName& _ID, const EKLDebugWindowTypes _WindowType);

    [[nodiscard]] const FName& GetID() const;
    [[nodiscard]] uint16 GetIndex() const;
    [[nodiscard]] EKLDebugWindowTypes GetWindowType() const;

protected:
    // TKLDebugNetworkingMessage_Base
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) override;
    // TKLDebugNetworkingMessage_Base

protected:
    FName mID;
    uint16 mIndex = TNumericLimits<uint16>::Max();
    EKLDebugWindowTypes mWindowType = EKLDebugWindowTypes::Count;
};

template <EKLDebugImGuiNetworkMessage MessageType>
TKLDebugNetworkingMessage_WindowBase<MessageType>::TKLDebugNetworkingMessage_WindowBase(const uint16 _NewSelection, const FName& _ID, const EKLDebugWindowTypes _WindowType)
    : mID(_ID)
    , mIndex(_NewSelection)
    , mWindowType(_WindowType)
{
}

template <EKLDebugImGuiNetworkMessage MessageType>
void TKLDebugNetworkingMessage_WindowBase<MessageType>::SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input)
{
    FArchive& Archive = _Input.GetArchive();

    Archive << mWindowType;
    Archive << mIndex;
    Archive << mID;
}

template <EKLDebugImGuiNetworkMessage MessageType>
inline const FName& TKLDebugNetworkingMessage_WindowBase<MessageType>::GetID() const
{
    return mID;
}

template <EKLDebugImGuiNetworkMessage MessageType>
inline uint16 TKLDebugNetworkingMessage_WindowBase<MessageType>::GetIndex() const
{
    return mIndex;
}

template <EKLDebugImGuiNetworkMessage MessageType>
inline EKLDebugWindowTypes TKLDebugNetworkingMessage_WindowBase<MessageType>::GetWindowType() const
{
    return mWindowType;
}
