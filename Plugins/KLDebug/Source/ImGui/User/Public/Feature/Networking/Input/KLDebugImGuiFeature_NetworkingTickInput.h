// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "ImGui/User/Internal/Feature/Interface/KLDebugImGuiFeatureInterfaceBase.h"

// engine
#include "Engine/World.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Serialization/Archive.h"
#include "Templates/UnrealTemplate.h"

class KLDEBUGIMGUIUSER_API FKLDebugImGuiFeature_NetworkingTickInput final : public FNoncopyable
{
public:
    explicit FKLDebugImGuiFeature_NetworkingTickInput(const UWorld& _World, FArchive& _Archive);

    UE_NODISCARD const UWorld& GetWorld() const;
    UE_NODISCARD FArchive& GetWriter() const;

    void SetUpdateSceneProxy();
    UE_NODISCARD bool ShouldUpdateSceneProxy() const;

private:
    const UWorld& mWorld;
    FArchive& mArchive;
    bool mUpdateSceneProxy = false;
};

inline const UWorld& FKLDebugImGuiFeature_NetworkingTickInput::GetWorld() const
{
    return mWorld;
}

inline FArchive& FKLDebugImGuiFeature_NetworkingTickInput::GetWriter() const
{
    return mArchive;
}

inline void FKLDebugImGuiFeature_NetworkingTickInput::SetUpdateSceneProxy()
{
    mUpdateSceneProxy = true;
}

inline bool FKLDebugImGuiFeature_NetworkingTickInput::ShouldUpdateSceneProxy() const
{
    return mUpdateSceneProxy;
}