// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/CachedConnection/KLDebugNetworkingConnectionGetPendingBuffer.h"

FKLDebugNetworkingConnectionGetPendingBuffer::FKLDebugNetworkingConnectionGetPendingBuffer(TArray<FKLDebugNetworkingPendingMessage>& _PendingMessages, bool& _HasReadData)
    : mPendingMessages(_PendingMessages)
    , mHasReadData(_HasReadData)
{
}

FKLDebugNetworkingConnectionGetPendingBuffer::~FKLDebugNetworkingConnectionGetPendingBuffer()
{
    if (mPendingMessages.IsEmpty())
    {
        mHasReadData = false;
    }
}
