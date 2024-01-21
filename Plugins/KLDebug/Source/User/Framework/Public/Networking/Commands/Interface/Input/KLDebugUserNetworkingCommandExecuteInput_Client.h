// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Internal/Networking/Commands/Interface/Input/KLDebugUserNetworkingCommandExecuteInput_Base.h"

class UWorld;

class KLDEBUGUSERFRAMEWORK_API FKLDebugUserNetworkingCommandExecuteInput_Client final : public FKLDebugUserNetworkingCommandExecuteInput_Base
{
public:
    explicit FKLDebugUserNetworkingCommandExecuteInput_Client(UWorld& _World);
};
