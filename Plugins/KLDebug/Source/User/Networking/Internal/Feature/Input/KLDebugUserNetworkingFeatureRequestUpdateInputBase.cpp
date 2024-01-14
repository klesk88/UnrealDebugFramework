// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Internal/Feature/Input/KLDebugUserNetworkingFeatureRequestUpdateInputBase.h"

FKLDebugUserNetworkingFeatureRequestUpdateInputBase::FKLDebugUserNetworkingFeatureRequestUpdateInputBase(const UWorld& _World, FArchive& _ReaderArchive, FArchive& _WriterArchive)
    : mWorld(_World)
    , mReaderArchive(_ReaderArchive)
    , mWriterArchive(_WriterArchive)
{
}
