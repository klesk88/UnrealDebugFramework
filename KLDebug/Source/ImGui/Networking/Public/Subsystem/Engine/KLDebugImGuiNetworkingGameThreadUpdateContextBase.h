// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "Engine/World.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/ObjectKey.h"

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingGameThreadUpdateContextBase : public FNoncopyable
{
public:
    virtual ~FKLDebugImGuiNetworkingGameThreadUpdateContextBase();

    void SetRemovedWorlds(const TArray<FObjectKey>& _RemovedWorlds);
    UE_NODISCARD const TArray<FObjectKey>& GetRemovedWorlds() const;
    UE_NODISCARD TArray<const UWorld*>& GetWorldsMutable();
    UE_NODISCARD const TArray<const UWorld*>& GetWorlds() const;

    void SetShouldKeepTicking(const bool _ShouldTick);
    UE_NODISCARD bool GetShouldKeepTicking() const;

private:
    TArray<const UWorld*> mWorlds;
    TArray<FObjectKey> mRemovedWorlds;
    bool mShouldTick = false;
};

inline TArray<const UWorld*>& FKLDebugImGuiNetworkingGameThreadUpdateContextBase::GetWorldsMutable()
{
    return mWorlds;
}

inline const TArray<const UWorld*>& FKLDebugImGuiNetworkingGameThreadUpdateContextBase::GetWorlds() const
{
    return mWorlds;
}

inline void FKLDebugImGuiNetworkingGameThreadUpdateContextBase::SetShouldKeepTicking(const bool _ShouldTick)
{
    mShouldTick = _ShouldTick;
}

inline bool FKLDebugImGuiNetworkingGameThreadUpdateContextBase::GetShouldKeepTicking() const
{
    return mShouldTick;
}

inline void FKLDebugImGuiNetworkingGameThreadUpdateContextBase::SetRemovedWorlds(const TArray<FObjectKey>& _RemovedWorlds)
{
    mRemovedWorlds = _RemovedWorlds;
}

inline const TArray<FObjectKey>& FKLDebugImGuiNetworkingGameThreadUpdateContextBase::GetRemovedWorlds() const
{
    return mRemovedWorlds;
}