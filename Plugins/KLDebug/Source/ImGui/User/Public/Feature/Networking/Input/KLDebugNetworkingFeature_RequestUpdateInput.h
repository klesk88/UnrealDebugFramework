// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"

// engine
#include "Engine/World.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Serialization/Archive.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGIMGUIUSER_API FKLDebugNetworkingFeature_RequestUpdateInput final : public FNoncopyable
{
public:
    explicit FKLDebugNetworkingFeature_RequestUpdateInput(const UWorld& _World, FArchive& _ReaderArchive, FArchive& _WriterArchive);

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD FArchive& GetWriter() const;
    UE_NODISCARD FArchive& GetReader() const;

private:
    const UWorld& mWorld;
    FArchive& mReaderArchive;
    FArchive& mWriterArchive;
};

inline const UWorld& FKLDebugNetworkingFeature_RequestUpdateInput::GetWorld() const
{
    return mWorld;
}

inline FArchive& FKLDebugNetworkingFeature_RequestUpdateInput::GetWriter() const
{
    return mWriterArchive;
}

inline FArchive& FKLDebugNetworkingFeature_RequestUpdateInput::GetReader() const
{
    return mReaderArchive;
}