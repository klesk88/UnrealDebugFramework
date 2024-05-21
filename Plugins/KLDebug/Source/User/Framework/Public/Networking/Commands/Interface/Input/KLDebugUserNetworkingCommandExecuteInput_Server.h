// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Commands/Interface/Input/KLDebugUserNetworkingCommandExecuteInput_Base.h"

// engine
#include "Containers/Array.h"
#include "GenericPlatform/GenericPlatformProcess.h"

class APlayerController;
class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingCommandExecuteInput_Server final : public FKLDebugUserNetworkingCommandExecuteInput_Base
{
public:
    explicit FKLDebugUserNetworkingCommandExecuteInput_Server(APlayerController& _PlayerClientOwner, UWorld& _World);

    [[nodiscard]] const APlayerController& GetOwningPlayerConnection() const;
    [[nodiscard]] APlayerController& GetOwningPlayerConnectionMutable() const;

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