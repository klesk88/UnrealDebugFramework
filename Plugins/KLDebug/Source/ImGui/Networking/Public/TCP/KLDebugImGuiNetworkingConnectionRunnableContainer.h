// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "HAL/Platform.h"
#include "HAL/RunnableThread.h"

template <typename ConnectionType>
class TKLDebugImGuiNetworkingConnectionRunnableContainer final : public FNoncopyable
{
public:
    ~TKLDebugImGuiNetworkingConnectionRunnableContainer();

    [[nodiscard]] ConnectionType& GetConnectionMutable();
    [[nodiscard]] const ConnectionType& GetConnection() const;

    void InitSocket(const TCHAR* _ThreadName);
    void ClearConnection();
    [[nodiscard]] bool HasValidConnection() const;

private:
    ConnectionType mConnection;
    FRunnableThread* mThread = nullptr;
};

template <typename ConnectionType>
TKLDebugImGuiNetworkingConnectionRunnableContainer<ConnectionType>::~TKLDebugImGuiNetworkingConnectionRunnableContainer()
{
    ClearConnection();
}

template <typename ConnectionType>
void TKLDebugImGuiNetworkingConnectionRunnableContainer<ConnectionType>::ClearConnection()
{
    QUICK_SCOPE_CYCLE_COUNTER(KLDebugImGuiNetworkingConnectionRunnableContainer_ClearConnection);

    if (mThread)
    {
        mConnection.Stop();
        mThread->WaitForCompletion();
        delete mThread;
        mThread = nullptr;
    }
}

template <typename ConnectionType>
void TKLDebugImGuiNetworkingConnectionRunnableContainer<ConnectionType>::InitSocket(const TCHAR* _ThreadName)
{
    mThread = FRunnableThread::Create(&mConnection, _ThreadName, 128 * 1024, TPri_Normal);
}

template <typename ConnectionType>
inline const ConnectionType& TKLDebugImGuiNetworkingConnectionRunnableContainer<ConnectionType>::GetConnection() const
{
    return mConnection;
}

template <typename ConnectionType>
inline ConnectionType& TKLDebugImGuiNetworkingConnectionRunnableContainer<ConnectionType>::GetConnectionMutable()
{
    return mConnection;
}

template <typename ConnectionType>
inline bool TKLDebugImGuiNetworkingConnectionRunnableContainer<ConnectionType>::HasValidConnection() const
{
    return mThread != nullptr;
}