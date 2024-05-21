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

    KLDEBUGNETWORKINGRUNTIME_API void Init(const uint32 _MaxMessageSizeSplit, const uint32 _MaxDataSizeBeforeCompression, const uint32 _CompressMinBytesSaved, const uint32 _CompressMinPercentSaved);

    [[nodiscard]] KLDEBUGNETWORKINGRUNTIME_API MessageID GetNewMessageID();
    [[nodiscard]] KLDEBUGNETWORKINGRUNTIME_API uint32 GetHeaderSize();

    [[nodiscard]] KLDEBUGNETWORKINGRUNTIME_API bool CompressBuffer(const TArray<uint8>& _DataToCompress, TArray<uint8>& _CompressedData);
    KLDEBUGNETWORKINGRUNTIME_API bool UncompressBuffer(const uint32 _UncompressSize, const TArrayView<const uint8>& _CompressedData, TArrayView<uint8>& _UncompressedData);

    KLDEBUGNETWORKINGRUNTIME_API void PrepareMessageToSend_Uncompressed(const IKLDebugNetworkingMessageInterface& _Message, TArray<uint8>& _DataToSend, FArchive& _Archive);
    KLDEBUGNETWORKINGRUNTIME_API void PrepareMessageToSend_SplitIfRequired(const IKLDebugNetworkingMessageInterface& _Message, TArray<uint8>& _DataToSend, TArray<uint8>& _CompressDataBuffer, FArchive& _Archive);

    // helper function that given a buffer of data to read (_ReadBuffer) and a temp buffer where to store the message content (_MessageBufferData) invokes the _Callback
    // for each message extrapolate from the _ReadBuffer
    KLDEBUGNETWORKINGRUNTIME_API uint32 ReadBufferGetStopReadLocation(const ReadBufferCallback& _Callback, TArray<uint8>& _MessageBufferData, FArchive& _ReadBuffer);

}    // namespace KL::Debug::Networking::Message