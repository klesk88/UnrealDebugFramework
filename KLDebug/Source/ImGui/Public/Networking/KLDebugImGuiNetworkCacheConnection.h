#pragma once

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatform.h"
#include "HAL/Platform.h"
#include "Templates/RefCounting.h"

class FKLDebugImGuiNetworkManager_Base;
class FSocket;

//based on FEditorDomainSaveServer::FClientConnection
class KLDEBUGIMGUI_API FKLDebugImGuiNetworkCacheConnection final : public FRefCountedObject
{
public:
    explicit FKLDebugImGuiNetworkCacheConnection(FSocket& _ClientSocket);
    ~FKLDebugImGuiNetworkCacheConnection();

    /** Release resources and indicate this connection should no longer be used. */
    void Shutdown();

    UE_NODISCARD bool IsValid() const;
    UE_NODISCARD FSocket& GetSocket() const;

private:
    /** The socket to the client process, returned from accept on the server's ListenSocket. */
    FSocket* mClientSocket = nullptr;
    TArray<uint8> MessageBuffer;
    int32 BufferOffset = 0;
};

inline bool FKLDebugImGuiNetworkCacheConnection::IsValid() const
{
    return mClientSocket != nullptr;
}

inline FSocket& FKLDebugImGuiNetworkCacheConnection::GetSocket() const
{
    return *mClientSocket;
}