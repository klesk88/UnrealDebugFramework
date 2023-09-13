#include "Networking/KLDebugImGuiNetworkCacheConnection.h"

//engine
#include "Sockets.h"
#include "SocketSubsystem.h"

FKLDebugImGuiNetworkCacheConnection::FKLDebugImGuiNetworkCacheConnection(FSocket& _ClientSocket)
    : mClientSocket(&_ClientSocket)
{
}

FKLDebugImGuiNetworkCacheConnection::~FKLDebugImGuiNetworkCacheConnection()
{
    Shutdown();
}

void FKLDebugImGuiNetworkCacheConnection::Shutdown()
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
