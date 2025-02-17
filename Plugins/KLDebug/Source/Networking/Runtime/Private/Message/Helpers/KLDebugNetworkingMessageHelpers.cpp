// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Message/Helpers/KLDebugNetworkingMessageHelpers.h"

#include "Log/KLDebugNetworkingLog.h"
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
    static uint32 MessageMaxDataSize = 500;
    static float MessageMaxDataSizeInv = 1.f / static_cast<float>(MessageMaxDataSize);
    static uint32 MessageMaxDataSizeBeforeCompression = 500;
    static int32 CompressMinBytesSaved = 0;
    static int32 CompressMinPercentSaved = 0;

    void PrepareMessageToSend(const IKLDebugNetworkingMessageInterface& _Message, const bool _IsCompressed, const uint32 _MessageTotalUncompressedSize, const uint32 _MessageTotalCompressSize, const uint32 _MessageDataOffset, const uint32 _StartCompressDataOffset, const KL::Debug::Networking::Message::MessageID _MessageID, TArrayView<uint8>& _DataToSend, FArchive& _Archive)
    {
        ensureMsgf(_Message.SupportsEmptyData() || !_DataToSend.IsEmpty(), TEXT("we expect some data to send"));

        FKLDebugNetworkingMessage_Header Header{ _Message.GetMessageType(), _Message.GetMessageEnumType(), static_cast<uint32>(_DataToSend.Num()), _MessageTotalUncompressedSize, _IsCompressed, _MessageTotalCompressSize, _StartCompressDataOffset, _MessageDataOffset, _MessageID };
        Header.Serialize(_Archive);
        _Archive.Serialize(_DataToSend.GetData(), _DataToSend.Num());
    }

    bool CompressBuffer(const TArray<uint8>& _DataToCompress, TArray<uint8>& _CompressedData)
    {
        // based on FGameplayDebuggerDataPack::RequestReplication

        const uint32 UncompressSize = _DataToCompress.Num();
        int32 CompressedSize = FCompression::CompressMemoryBound(NAME_Zlib, _DataToCompress.Num(), COMPRESS_BiasMemory);
        _CompressedData.SetNum(CompressedSize, EAllowShrinking::No);
        const bool Result = FCompression::CompressMemoryIfWorthDecompressing(NAME_Zlib, CompressMinBytesSaved, CompressMinPercentSaved, _CompressedData.GetData(), CompressedSize, _DataToCompress.GetData(), UncompressSize, COMPRESS_BiasMemory);
        if (Result)
        {
            _CompressedData.SetNum(CompressedSize, EAllowShrinking::No);
            return true;
        }
        else
        {
            _CompressedData.Reset();
            return false;
        }
    }

    ///////////////////////////////////////////////////
    /// public

    void Init(const uint32 _MaxMessageSizeSplit, const uint32 _MaxDataSizeBeforeCompression, const uint32 _CompressMinBytesSaved, const uint32 _CompressMinPercentSaved)
    {
        if (HeaderSize != 0)
        {
            return;
        }

        TArray<uint8> TempArray;
        TempArray.Reserve(34);
        FMemoryWriter Writer(TempArray);
        FKLDebugNetworkingMessage_Header DummyHeaderMessage{ 0, 0, 0, 0, false, 0, 0, 0, 0 };
        DummyHeaderMessage.Serialize(Writer);
        HeaderSize = TempArray.Num();

        MessageMaxDataSize = _MaxMessageSizeSplit;
        MessageMaxDataSizeInv = 1.f / static_cast<float>(_MaxMessageSizeSplit);

        MessageMaxDataSizeBeforeCompression = _MaxDataSizeBeforeCompression;

        CompressMinBytesSaved = static_cast<int32>(_CompressMinBytesSaved);
        CompressMinPercentSaved = static_cast<int32>(_CompressMinPercentSaved);
    }

    uint32 GetHeaderSize()
    {
        return HeaderSize;
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

    void PrepareMessageToSend_Uncompressed(const IKLDebugNetworkingMessageInterface& _Message, TArray<uint8>& _DataToSend, FArchive& _Archive)
    {
        TArrayView<uint8> MessageDataView(_DataToSend.GetData(), _DataToSend.Num());
        PrepareMessageToSend(_Message, false, _DataToSend.Num(), 0, 0, 0, 0, MessageDataView, _Archive);
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

            _MessageBufferData.SetNum(static_cast<int32>(HeaderMessage.GetMessageDataSize()), EAllowShrinking::No);
            _Reader.Serialize(_MessageBufferData.GetData(), HeaderMessage.GetMessageDataSize());
            CurrentPosition = _Reader.Tell();

            FMemoryReader MessageData(_MessageBufferData);
            _Callback(HeaderMessage, MessageData);
        }

        checkf(CurrentPosition < TNumericLimits<uint32>::Max(), TEXT("current position too high not expected"));
        return static_cast<uint32>(CurrentPosition);
    }

    void PrepareMessageToSend_SplitMessage(const IKLDebugNetworkingMessageInterface& _Message, const bool _IsCompressed, const uint32 _CompressTotalSize, const uint32 _UncompressTotalSize, TArray<uint8>& _Data, FArchive& _Archive)
    {
        const int32 MessagesCount = FMath::CeilToInt(static_cast<float>(_Data.Num()) * MessageMaxDataSizeInv);
        checkf(MessagesCount > 1, TEXT("we should have more then one message"));
        const MessageID MessageID = GetNewMessageID();
        uint32 MessageOffset = 0;

#if DO_ENSURE
        uint32 EnsureTotalSizeSent = 0;
#endif

        for (int32 i = 0; i < MessagesCount; ++i)
        {
            const int32 StartIndex = i * MessageMaxDataSize;
            const int32 EndIndex = FMath::Min((i + 1) * static_cast<int32>(MessageMaxDataSize), _Data.Num());
            checkf(EndIndex <= _Data.Num() && StartIndex < _Data.Num() && StartIndex < EndIndex, TEXT("going out of bounds"));
            TArrayView<uint8> MessageDataView(&_Data[StartIndex], EndIndex - StartIndex);

            PrepareMessageToSend(_Message,
                _IsCompressed,
                _UncompressTotalSize,
                _CompressTotalSize,
                MessageOffset,
                0,
                MessageID,
                MessageDataView,
                _Archive);

            MessageOffset += MessageDataView.Num();

#if DO_ENSURE
            EnsureTotalSizeSent += MessageDataView.Num();
#endif
        }

#if DO_ENSURE
        ensureMsgf(_IsCompressed ? EnsureTotalSizeSent == _CompressTotalSize : EnsureTotalSizeSent == _UncompressTotalSize, TEXT("we sent more or less bytes then exptected"));
#endif
    }

    void PrepareMessageToSend_SplitIfRequired(const IKLDebugNetworkingMessageInterface& _Message, TArray<uint8>& _DataToSend, TArray<uint8>& _CompressDataBuffer, FArchive& _Archive)
    {
        if (_DataToSend.IsEmpty())
        {
            ensureMsgf(false, TEXT("we expect some data to send"));
            return;
        }

        _CompressDataBuffer.Reset();
        TArray<uint8>* BufferToUse = &_DataToSend;
        bool IsCompressed = false;
        const uint32 UncompressSize = static_cast<uint32>(_DataToSend.Num());
        if (static_cast<uint32>(_DataToSend.Num()) > MessageMaxDataSizeBeforeCompression)
        {
            if (CompressBuffer(_DataToSend, _CompressDataBuffer))
            {
                IsCompressed = true;
                if (_CompressDataBuffer.IsEmpty())
                {
                    UE_LOG(LogKLDebug_Networking, Error, TEXT("PrepareMessageToSend_SplitIfRequired>> Compressing data failed, returned an empty data buffer"));
                    return;
                }

                BufferToUse = &_CompressDataBuffer;
                UE_LOG(LogKLDebug_Networking, Display, TEXT("PrepareMessageToSend_SplitIfRequired>> Compressing data. Original size [%d] compress size [%d]"), UncompressSize, _CompressDataBuffer.Num());
            }
            else
            {
                _CompressDataBuffer.Reset();
            }
        }

        const uint32 CompressSize = static_cast<uint32>(_CompressDataBuffer.Num());
        if (static_cast<uint32>(BufferToUse->Num()) > MessageMaxDataSize)
        {
            PrepareMessageToSend_SplitMessage(_Message, IsCompressed, CompressSize, UncompressSize, *BufferToUse, _Archive);
        }
        else
        {
            TArrayView<uint8> MessageDataView(BufferToUse->GetData(), BufferToUse->Num());
            PrepareMessageToSend(_Message, IsCompressed, UncompressSize, CompressSize, 0, 0, 0, MessageDataView, _Archive);
        }
    }

}    // namespace KL::Debug::Networking::Message