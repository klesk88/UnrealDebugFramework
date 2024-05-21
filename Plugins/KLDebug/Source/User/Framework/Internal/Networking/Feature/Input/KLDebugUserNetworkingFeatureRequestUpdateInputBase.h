// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Templates/UnrealTemplate.h"

class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingFeatureRequestUpdateInputBase : public FNoncopyable
{
public:
    explicit FKLDebugUserNetworkingFeatureRequestUpdateInputBase(const UWorld& _World, FArchive& _ReaderArchive, FArchive& _WriterArchive);

    [[nodiscard]] const UWorld& GetWorld() const;
    [[nodiscard]] FArchive& GetWriter() const;
    [[nodiscard]] FArchive& GetReader() const;

private:
    const UWorld& mWorld;
    FArchive& mReaderArchive;
    FArchive& mWriterArchive;
};

inline const UWorld& FKLDebugUserNetworkingFeatureRequestUpdateInputBase::GetWorld() const
{
    return mWorld;
}

inline FArchive& FKLDebugUserNetworkingFeatureRequestUpdateInputBase::GetWriter() const
{
    return mWriterArchive;
}

inline FArchive& FKLDebugUserNetworkingFeatureRequestUpdateInputBase::GetReader() const
{
    return mReaderArchive;
}