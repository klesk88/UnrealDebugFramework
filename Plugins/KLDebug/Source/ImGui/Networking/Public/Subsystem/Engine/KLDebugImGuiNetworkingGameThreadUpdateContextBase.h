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

    void SetNewWorlds(const TArray<TWeakObjectPtr<const UWorld>>& _NewWorlds);
    void SetRemovedWorlds(const TArray<FObjectKey>& _RemovedWorlds);
    [[nodiscard]] const TArray<FObjectKey>& GetRemovedWorlds() const;
    [[nodiscard]] TArray<const UWorld*>& GetWorldsMutable();
    [[nodiscard]] const TArray<const UWorld*>& GetWorlds() const;
    [[nodiscard]] const TArray<TWeakObjectPtr<const UWorld>>& GetNewWorlds() const;

    void SetShouldKeepTicking(const bool _ShouldTick);
    [[nodiscard]] bool GetShouldKeepTicking() const;

private:
    TArray<const UWorld*> mWorlds;
    TArray<TWeakObjectPtr<const UWorld>> mNewWorlds;
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

inline const TArray<TWeakObjectPtr<const UWorld>>& FKLDebugImGuiNetworkingGameThreadUpdateContextBase::GetNewWorlds() const
{
    return mNewWorlds;
}