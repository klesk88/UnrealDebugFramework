// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Networking/Window/Input/KLDebugNetworkingWindowInputBase.h"

class UObject;

class KLDEBUGUSERFRAMEWORK_API FKLDebugNetworkingWindowRequestUpdateInput final : public FKLDebugNetworkingWindowInputBase
{
public:
    explicit FKLDebugNetworkingWindowRequestUpdateInput(const UWorld& _World, IKLDebugContextInterface* _Contex, FArchive& _ReaderArchive, FArchive& _WriterArchive);

    [[nodiscard]] FArchive& GetWriter() const;

private:
    FArchive& mWriterArchive;
};

inline FArchive& FKLDebugNetworkingWindowRequestUpdateInput::GetWriter() const
{
    return mWriterArchive;
}
