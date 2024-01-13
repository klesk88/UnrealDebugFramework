// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Feature/Networking/Input/KLDebugNetworkingFeature_RequestUpdateInput.h"

FKLDebugNetworkingFeature_RequestUpdateInput::FKLDebugNetworkingFeature_RequestUpdateInput(const UWorld& _World, FArchive& _ReaderArchive, FArchive& _WriterArchive)
    : mWorld(_World)
    , mReaderArchive(_ReaderArchive)
    , mWriterArchive(_WriterArchive)
{
}
