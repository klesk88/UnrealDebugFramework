// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Templates/UnrealTemplate.h"

class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingCommandExecuteInput_Base : public FNoncopyable
{
public:
    explicit FKLDebugUserNetworkingCommandExecuteInput_Base(UWorld& _World);
    virtual ~FKLDebugUserNetworkingCommandExecuteInput_Base() = default;

    UE_NODISCARD UWorld& GetWorldMutable() const;
    UE_NODISCARD const UWorld& GetWorld() const;

private:
    UWorld& mWorld;
};

inline UWorld& FKLDebugUserNetworkingCommandExecuteInput_Base::GetWorldMutable() const
{
    return mWorld;
}

inline const UWorld& FKLDebugUserNetworkingCommandExecuteInput_Base::GetWorld() const
{
    return mWorld;
}