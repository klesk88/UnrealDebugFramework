// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Helpers/KLDebugNetworkingMessageHelpers.h"

#include "Message/Header/KLDebugNetworkingMessage_Header.h"
#include "Message/KLDebugNetworkingMessageInterface.h"

// engine
#include "Containers/Array.h"
#include "Math/NumericLimits.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/Compression.h"
#include "Serialization/Archive.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"

namespace KL::Debug::Networking::Message
{
    ///////////////////////////////////////////////////
    /// private

    static int32 HeaderSize = 0;
    static MessageID CurrentMessageID = 0;

    ///////////////////////////////////////////////////
    /// public

    void InitHeaderSize()
    {
        if (HeaderSize == 0)
        {
            TArray<uint8> TempArray;
            TempArray.Reserve(34);
            FMemoryWriter Writer(TempArray);
            FKLDebugNetworkingMessage_Header DummyHeaderMessage{ 0, 0, 0, 0, false, 0, 0, 0, 0 };
            DummyHeaderMessage.Serialize(Writer);
            HeaderSize = TempArray.Num();
        }
    }

    uint32 GetHeaderSize()
    {
        return HeaderSize;
    }

    void CompressBuffer(const TArray<uint8>& _DataToCompress, TArray<uint8>& _CompressedData)
    {
        // based on FGameplayDebuggerDataPack::RequestReplication

        uint32 CompressSize = 0;
        const uint32 UncompressSize = _DataToCompress.Num();
        int32 CompressedSize = FMath::TruncToInt(1.1f * static_cast<float>(UncompressSize));
        _CompressedData.SetNum(CompressedSize);
        FCompression::CompressMemory(NAME_Zlib, _CompressedData.GetData(), CompressedSize, _DataToCompress.GetData(), UncompressSize, COMPRESS_BiasMemory);
        _CompressedData.SetNum(CompressedSize, false);
    }

    bool UncompressBuffer(const uint32 _UncompressSize, const TArrayView<const uint8>& _CompressedData, TArrayView<uint8>& _UncompressedData)
    {
        // based on FGameplayDebuggerDataPack::OnReplicated
        ensureMsgf(!_UncompressedData.IsEmpty(), TEXT("we expect this to be already initialize currently from the caller"));
        checkf(_UncompressSize < static_cast<uint32>(TNumericLimits<int32>::Max()), TEXT("too much data"));

        const uint8* CompressedBuffer = _CompressedData.GetData();
        const int32 CompressedSize = _CompressedData.Num();
        return FCompression::UncompressMemory(NAME_Zlib, _UncompressedData.GetData(), static_cast<int32>(_UncompressSize), CompressedBuffer, CompressedSize);
    }

    void PrepareMessageToSend(const IKLDebugNetworkingMessageInterface& _Message, const bool _IsCompressed, const uint32 _MessageTotalUncompressedSize, const uint32 _MessageTotalCompressSize, const uint32 _MessageDataOffset, const uint32 _StartCompressDataOffset, const KL::Debug::Networking::Message::MessageID _MessageID, TArray<uint8>& _DataToSend, FArchive& _Archive)
    {
        ensureMsgf(_Message.SupportsEmptyData() || !_DataToSend.IsEmpty(), TEXT("we expect some data to send"));

        FKLDebugNetworkingMessage_Header Header{ _Message.GetMessageType(), _Message.GetMessageEnumType(), static_cast<uint32>(_DataToSend.Num()), _MessageTotalUncompressedSize, _IsCompressed, _MessageTotalCompressSize, _StartCompressDataOffset, _MessageDataOffset, _MessageID };
        Header.Serialize(_Archive);
        _Archive.Serialize(_DataToSend.GetData(), _DataToSend.Num());
    }

    MessageID GetNewMessageID()
    {
        CurrentMessageID = static_cast<uint16>((static_cast<uint32>(CurrentMessageID) + 1) % TNumericLimits<MessageID>::Max());
        return CurrentMessageID;
    }

    uint32 ReadBufferGetStopReadLocation(const ReadBufferCallback& _Callback, TArray<uint8>& _MessageBufferData, FArchive& _Reader)
    {
        const int64 TotalSize = _Reader.TotalSize();
        int64 CurrentPosition = _Reader.Tell();

        while (!_Reader.AtEnd())
        {
            if (TotalSize - CurrentPosition < GetHeaderSize())
            {
                // not enough data for the header
                break;
            }

            const FKLDebugNetworkingMessage_Header HeaderMessage{ _Reader };
            if (!HeaderMessage.IsValid())
            {
                // garbage in the stream skip one byte
                _Reader.Seek(++CurrentPosition);
                // clear the error in the archive otherwise the archvie stops to read data so we just bypass bytes
                _Reader.ClearError();
                continue;
            }

            const int64 CurrentReadBufferPosition = _Reader.Tell();
            const int64 RemainingSpace = TotalSize - CurrentReadBufferPosition;
            if (RemainingSpace < HeaderMessage.GetMessageDataSize())
            {
                break;
            }

            _MessageBufferData.SetNum(static_cast<int32>(HeaderMessage.GetMessageDataSize()), false);
            _Reader.Serialize(_MessageBufferData.GetData(), HeaderMessage.GetMessageDataSize());
            CurrentPosition = _Reader.Tell();

            FMemoryReader MessageData(_MessageBufferData);
            _Callback(HeaderMessage, MessageData);
        }

        checkf(CurrentPosition < TNumericLimits<uint32>::Max(), TEXT("current position too high not expected"));
        return static_cast<uint32>(CurrentPosition);
    }

}    // namespace KL::Debug::Networking::Message