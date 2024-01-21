// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Networking/Window/Input/KLDebugNetworkingWindowRequestUpdateInput.h"

FKLDebugNetworkingWindowRequestUpdateInput::FKLDebugNetworkingWindowRequestUpdateInput(const UWorld& _World, IKLDebugContextInterface* _Contex, FArchive& _ReaderArchive, FArchive& _WriterArchive)
    : FKLDebugNetworkingWindowInputBase(_World, _Contex, _ReaderArchive)
    , mWriterArchive(_WriterArchive)
{
}
