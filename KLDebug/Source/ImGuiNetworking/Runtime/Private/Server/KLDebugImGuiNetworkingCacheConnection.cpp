#include "Server/KLDebugImGuiNetworkingCacheConnection.h"

//engine
#include "Sockets.h"
#include "SocketSubsystem.h"

FKLDebugImGuiNetworkingCacheConnection::FKLDebugImGuiNetworkingCacheConnection(FSocket& _ClientSocket)
    : mClientSocket(&_ClientSocket)
{
}

FKLDebugImGuiNetworkingCacheConnection::~FKLDebugImGuiNetworkingCacheConnection()
{
    Shutdown();
}

void FKLDebugImGuiNetworkingCacheConnection::Shutdown()
{
    if (mClientSocket)
    {
        ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
        check(SocketSubsystem);
        mClientSocket->Close();
        SocketSubsystem->DestroySocket(mClientSocket);
        mClientSocket = nullptr;
    }
}
