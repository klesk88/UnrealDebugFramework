// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/KLDebugImGuiNetworkingMessageTypes.h"

// engine
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"

class FArchive;
class IKLDebugNetworkingMessageInterface;

namespace KL::Debug::ImGuiNetworking::Message
{
    KLDEBUGIMGUINETWORKING_API void CompressBuffer(const TArray<uint8>& _DataToCompress, TArray<uint8>& _CompressedData);
    KLDEBUGIMGUINETWORKING_API bool UncompressBuffer(const uint32 _UncompressSize, const TArrayView<const uint8>& _CompressedData, TArrayView<uint8>& _UncompressedData);
    KLDEBUGIMGUINETWORKING_API void PrepareMessageToSend(const IKLDebugNetworkingMessageInterface& _Message, const bool _IsCompressed, const uint32 _MessageTotalUncompressedSize, const uint32 _MessageTotalCompressSize, const uint32 _MessageDataOffset, const uint32 _StartCompressDataOffset, const KL::Debug::ImGui::Networking::Message::MessageID _MessageID, TArray<uint8>& _DataToSend, FArchive& _Archive);

    inline KLDEBUGIMGUINETWORKING_API void PrepareMessageToSend_Uncompressed(const IKLDebugNetworkingMessageInterface& _Message, const KL::Debug::ImGui::Networking::Message::MessageID _MessageID, const uint32 _TotalUncompressedMessageSize, const uint32 _MessageDataOffset, TArray<uint8>& _DataToSend, FArchive& _Archive)
    {
        PrepareMessageToSend(_Message, false, _DataToSend.Num(), 0, 0, 0, _MessageID, _DataToSend, _Archive);
    }

    inline KLDEBUGIMGUINETWORKING_API void PrepareMessageToSend_Uncompressed(const IKLDebugNetworkingMessageInterface& _Message, TArray<uint8>& _DataToSend, FArchive& _Archive)
    {
        PrepareMessageToSend_Uncompressed(_Message, 0, static_cast<uint32>(_DataToSend.Num()), 0, _DataToSend, _Archive);
    }
}    // namespace KL::Debug::ImGuiNetworking::Message