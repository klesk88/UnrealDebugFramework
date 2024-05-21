// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/Window/KLDebugNetworkingMessage_WindowBase.h"

class FArchive;

class KLDEBUGIMGUINETWORKING_API FKLDebugNetworkingMessage_WindowToogle final : public TKLDebugNetworkingMessage_WindowBase<EKLDebugImGuiNetworkMessage::Client_WindowToogle>
{
public:
    explicit FKLDebugNetworkingMessage_WindowToogle() = default;
    explicit FKLDebugNetworkingMessage_WindowToogle(const uint16 _NewSelection, const FName& _ID, const EKLDebugWindowTypes _WindowType);
    explicit FKLDebugNetworkingMessage_WindowToogle(const EKLDebugWindowTypes _WindowType);

    [[nodiscard]] bool Server_GetHasElement() const;

private:
    // TKLDebugNetworkingMessage_Base
    void SerializeChild(const FKLDebugNetworkingMessageSerializeInput& _Input) final;
    // TKLDebugNetworkingMessage_Base

private:
    bool mHasElement = false;
};

inline bool FKLDebugNetworkingMessage_WindowToogle::Server_GetHasElement() const
{
    return mHasElement;
}