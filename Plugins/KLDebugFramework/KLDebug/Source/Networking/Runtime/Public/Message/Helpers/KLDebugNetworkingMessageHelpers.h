// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "GenericPlatform/GenericPlatformMisc.h"

class FArchive;
class FKLDebugNetworkingMessage_Header;
class IKLDebugNetworkingMessageInterface;

namespace KL::Debug::Networking::Message
{
    typedef uint16 MessageID;
    static constexpr const uint32 MessageMark = 0xB50FB50F;

    using ReadBufferCallback = TFunctionRef<void(const FKLDebugNetworkingMessage_Header& _MessageHeader, FArchive& _MessageData)>;

    KLDEBUGNETWORKINGRUNTIME_API UE_NODISCARD MessageID GetNewMessageID();
    KLDEBUGNETWORKINGRUNTIME_API void InitHeaderSize();
    KLDEBUGNETWORKINGRUNTIME_API UE_NODISCARD uint32 GetHeaderSize();

    KLDEBUGNETWORKINGRUNTIME_API void CompressBuffer(const TArray<uint8>& _DataToCompress, TArray<uint8>& _CompressedData);
    KLDEBUGNETWORKINGRUNTIME_API bool UncompressBuffer(const uint32 _UncompressSize, const TArrayView<const uint8>& _CompressedData, TArrayView<uint8>& _UncompressedData);
    KLDEBUGNETWORKINGRUNTIME_API void PrepareMessageToSend(const IKLDebugNetworkingMessageInterface& _Message, const bool _IsCompressed, const uint32 _MessageTotalUncompressedSize, const uint32 _MessageTotalCompressSize, const uint32 _MessageDataOffset, const uint32 _StartCompressDataOffset, const KL::Debug::Networking::Message::MessageID _MessageID, TArray<uint8>& _DataToSend, FArchive& _Archive);

    inline KLDEBUGNETWORKINGRUNTIME_API void PrepareMessageToSend_Uncompressed(const IKLDebugNetworkingMessageInterface& _Message, TArray<uint8>& _DataToSend, FArchive& _Archive)
    {
        PrepareMessageToSend(_Message, false, _DataToSend.Num(), 0, 0, 0, 0, _DataToSend, _Archive);
    }

    // helper function that given a buffer of data to read (_ReadBuffer) and a temp buffer where to store the message content (_MessageBufferData) invokes the _Callback
    // for each message extrapolate from the _ReadBuffer
    KLDEBUGNETWORKINGRUNTIME_API uint32 ReadBufferGetStopReadLocation(const ReadBufferCallback& _Callback, TArray<uint8>& _MessageBufferData, FArchive& _ReadBuffer);

}    // namespace KL::Debug::Networking::Message