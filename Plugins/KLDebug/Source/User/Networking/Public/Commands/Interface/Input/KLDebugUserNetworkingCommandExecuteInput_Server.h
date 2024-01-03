// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Commands/Interface/Input/KLDebugUserNetworkingCommandExecuteInput_Base.h"

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatformProcess.h"

class APlayerController;
class UWorld;

class KLDEBUGUSERNETWORKING_API FKLDebugUserNetworkingCommandExecuteInput_Server final : public FKLDebugUserNetworkingCommandExecuteInput_Base
{
public:
    explicit FKLDebugUserNetworkingCommandExecuteInput_Server(APlayerController& _PlayerClientOwner, UWorld& _World);

    UE_NODISCARD const APlayerController& GetOwningPlayerConnection() const;
    UE_NODISCARD APlayerController& GetOwningPlayerConnectionMutable() const;

private:
    APlayerController& mPlayerClientOwner;
};

inline const APlayerController& FKLDebugUserNetworkingCommandExecuteInput_Server::GetOwningPlayerConnection() const
{
    return mPlayerClientOwner;
}

inline APlayerController& FKLDebugUserNetworkingCommandExecuteInput_Server::GetOwningPlayerConnectionMutable() const
{
    return mPlayerClientOwner;
}