// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"

class FArchive;
class IKLDebugNetworkingMessageInterface;

namespace KL::Debug::Networking::Message
{
    typedef uint16 MessageID;
    static constexpr const uint32 MessageMark = 0xB50FB50F;

    static FString ArbitrerSessionEvent(TEXT("Local\\KLDebugArbitrer"));

    KLDEBUGNETWORKINGRUNTIME_API UE_NODISCARD MessageID
    GetNewMessageID();
    KLDEBUGNETWORKINGRUNTIME_API void InitHeaderSize();
    KLDEBUGNETWORKINGRUNTIME_API UE_NODISCARD uint32 GetHeaderSize();

    KLDEBUGNETWORKINGRUNTIME_API void CompressBuffer(const TArray<uint8>& _DataToCompress, TArray<uint8>& _CompressedData);
    KLDEBUGNETWORKINGRUNTIME_API bool UncompressBuffer(const uint32 _UncompressSize, const TArrayView<const uint8>& _CompressedData, TArrayView<uint8>& _UncompressedData);
    KLDEBUGNETWORKINGRUNTIME_API void PrepareMessageToSend(const IKLDebugNetworkingMessageInterface& _Message, const bool _IsCompressed, const uint32 _MessageTotalUncompressedSize, const uint32 _MessageTotalCompressSize, const uint32 _MessageDataOffset, const uint32 _StartCompressDataOffset, const KL::Debug::Networking::Message::MessageID _MessageID, TArray<uint8>& _DataToSend, FArchive& _Archive);

    inline KLDEBUGNETWORKINGRUNTIME_API void PrepareMessageToSend_Uncompressed(const IKLDebugNetworkingMessageInterface& _Message, const KL::Debug::Networking::Message::MessageID _MessageID, const uint32 _TotalUncompressedMessageSize, const uint32 _MessageDataOffset, TArray<uint8>& _DataToSend, FArchive& _Archive)
    {
        PrepareMessageToSend(_Message, false, _DataToSend.Num(), 0, 0, 0, _MessageID, _DataToSend, _Archive);
    }

    inline KLDEBUGNETWORKINGRUNTIME_API void PrepareMessageToSend_Uncompressed(const IKLDebugNetworkingMessageInterface& _Message, TArray<uint8>& _DataToSend, FArchive& _Archive)
    {
        PrepareMessageToSend_Uncompressed(_Message, 0, static_cast<uint32>(_DataToSend.Num()), 0, _DataToSend, _Archive);
    }
}    // namespace KL::Debug::Networking::Message