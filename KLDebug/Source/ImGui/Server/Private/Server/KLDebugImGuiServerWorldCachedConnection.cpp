// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugImGuiServerWorldCachedConnection.h"

// modules
#include "ImGui/Networking/Public/Settings/KLDebugImGuiNetworkingSettings.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerConnected.h"
#include "Networking/Arbitrer/Public/Messages/Server/KLDebugNetworkingArbitrerMessage_ServerDisconnected.h"
#include "Networking/Runtime/Public/Message/Header/KLDebugNetworkingMessageHeaderDefinitions.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"
#include "Utils/Public/KLDebugLog.h"

// engine
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Serialization/MemoryWriter.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

FKLDebugImGuiServerWorldCachedConnection::FKLDebugImGuiServerWorldCachedConnection(const UWorld& _World, const int32 _ServerPort, const int32 _DebugPort, FSocket& _WorldDebugSocket)
    : mWorldKey(&_World)
    , mServerPort(_ServerPort)
    , mDebugPort(_DebugPort)
    , mWorldDebugSocket(&_WorldDebugSocket)
{
}

FKLDebugImGuiServerWorldCachedConnection::~FKLDebugImGuiServerWorldCachedConnection()
{
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    check(SocketSubsystem);
    ShutdownInternal(*SocketSubsystem);
}

void FKLDebugImGuiServerWorldCachedConnection::OnRemove(const FInternetAddr& _ArbitrerAddress, FSocket& _ArbitrerSocket, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer)
{
    if (mState == EServerWorldConnectionState::MessageSent)
    {
        _WriteTempBuffer.Reset();
        _WriteBuffer.Reset();
        FKLDebugNetworkingArbitrerMessage_ServerDisconnected ServerMessage{ static_cast<uint32>(mServerPort) };
        FMemoryWriter TempWriter{ _WriteTempBuffer };
        ServerMessage.Serialize(TempWriter);
        FMemoryWriter Writer{ _WriteBuffer };
        KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(ServerMessage, _WriteTempBuffer, Writer);
        int32 DataSent = 0;
        if (_ArbitrerSocket.SendTo(_WriteBuffer.GetData(), _WriteBuffer.Num(), DataSent, _ArbitrerAddress))
        {
            mState = EServerWorldConnectionState::MessageToSend;
        }
    }
}

void FKLDebugImGuiServerWorldCachedConnection::ShutdownInternal(ISocketSubsystem& _SocketSubsystem)
{
    if (mWorldDebugSocket)
    {
        mWorldDebugSocket->Close();
        _SocketSubsystem.DestroySocket(mWorldDebugSocket);
        mWorldDebugSocket = nullptr;
    }
}

void FKLDebugImGuiServerWorldCachedConnection::RemoveInvalidClientConnections()
{
    for (int32 i = mCachedClientConnections.Num() - 1; i >= 0; --i)
    {
        const FKLDebugImGuiServerClientCachedConnection& ClientConnection = mCachedClientConnections[i];
        if (!ClientConnection.IsValid())
        {
            mCachedClientConnections.RemoveAtSwap(i, 1, false);
        }
    }
}

bool FKLDebugImGuiServerWorldCachedConnection::Parallel_Tick(const FInternetAddr& _ArbitrerAddress, FSocket& _ArbitrerSocket, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer)
{
    SendArbitrerMessageIfNeeded(_ArbitrerAddress, _ArbitrerSocket, _WriteTempBuffer, _WriteBuffer);
    TickPendingConnections();
    return TickClientsConnections();
}

void FKLDebugImGuiServerWorldCachedConnection::SendArbitrerMessageIfNeeded(const FInternetAddr& _ArbitrerAddress, FSocket& _ArbitrerSocket, TArray<uint8>& _WriteTempBuffer, TArray<uint8>& _WriteBuffer)
{
    switch (mState)
    {
    case EServerWorldConnectionState::MessageToSend:
    {
        _WriteTempBuffer.Reset();
        _WriteBuffer.Reset();
        FKLDebugNetworkingArbitrerMessage_ServerConnected ServerMessage{ static_cast<uint32>(mServerPort), static_cast<uint32>(mDebugPort) };
        FMemoryWriter TempWriter{ _WriteTempBuffer };
        ServerMessage.Serialize(TempWriter);
        FMemoryWriter Writer{ _WriteBuffer };
        KL::Debug::Networking::Message::PrepareMessageToSend_Uncompressed(ServerMessage, _WriteTempBuffer, Writer);
        int32 DataSent = 0;
        if (_ArbitrerSocket.SendTo(_WriteBuffer.GetData(), _WriteBuffer.Num(), DataSent, _ArbitrerAddress))
        {
            mState = EServerWorldConnectionState::MessageSent;
        }
        break;
    }
    case EServerWorldConnectionState::MessageSent:
        break;
    }
}

void FKLDebugImGuiServerWorldCachedConnection::TickPendingConnections()
{
    checkf(mWorldDebugSocket != nullptr, TEXT("must be valid"));

    const UKLDebugImGuiNetworkingSettings& Settings = UKLDebugImGuiNetworkingSettings::Get();

    bool bHasPendingConnection = false;
    while (mWorldDebugSocket->HasPendingConnection(bHasPendingConnection) && bHasPendingConnection)
    {
        FSocket* IncomingConnection = mWorldDebugSocket->Accept(TEXT("ClientRequest"));
        if (!IncomingConnection)
        {
            UE_LOG(LogKL_Debug, Error, TEXT("FKLDebugImGuiServerWorldCachedConnection::TickPendingConnections>> Could not accept socket"));
            continue;
        }

        int32 RecvSize = 0;
        int32 WriteSize = 0;
        IncomingConnection->SetReceiveBufferSize(static_cast<int32>(Settings.Server_GetReadBufferSize()), RecvSize);
        IncomingConnection->SetSendBufferSize(static_cast<int32>(Settings.Server_GetWriteBufferSize()), WriteSize);

        mCachedClientConnections.Emplace(RecvSize, WriteSize, *IncomingConnection);
    }
}

bool FKLDebugImGuiServerWorldCachedConnection::TickClientsConnections()
{
    bool HasNewReadData = false;
    for (int32 i = mCachedClientConnections.Num() - 1; i >= 0; --i)
    {
        FKLDebugImGuiServerClientCachedConnection& Connection = mCachedClientConnections[i];
        if (!Connection.IsValid())
        {
            UE_LOG(LogKL_Debug, Display, TEXT("FKLDebugImGuiServerWorldCachedConnection::TickClientsConnections>> Removing client connection because marked invalid"));
            mCachedClientConnections.RemoveAtSwap(i, 1, false);
            continue;
        }

        Connection.Tick();
        HasNewReadData |= Connection.HasNewReadData();
    }

    return HasNewReadData;
}

bool FKLDebugImGuiServerWorldCachedConnection::TickOnGameThread()
{
    const UWorld* World = Cast<const UWorld>(mWorldKey.ResolveObjectPtr());
    checkf(World != nullptr, TEXT("must be valid"));

    bool KeepTicking = false;
    for (int32 i = mCachedClientConnections.Num() - 1; i >= 0; --i)
    {
        FKLDebugImGuiServerClientCachedConnection& Connection = mCachedClientConnections[i];
        if (!Connection.IsValid())
        {
            UE_LOG(LogKL_Debug, Display, TEXT("FKLDebugImGuiServerWorldCachedConnection::TickOnGameThread>> Removing client connection because marked invalid"));
            mCachedClientConnections.RemoveAtSwap(i, 1, false);
            continue;
        }

        KeepTicking |= Connection.TickOnGameThread(*World);
    }

    return KeepTicking;
}
