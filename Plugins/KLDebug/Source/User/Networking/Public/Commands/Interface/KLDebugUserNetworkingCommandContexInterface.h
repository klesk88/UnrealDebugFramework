// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "CoreMinimal.h"
#include "Templates/UnrealTemplate.h"

class FArchive;

class KLDEBUGUSERNETWORKING_API IKLDebugUserNetworkingCommandContexInterface : public FNoncopyable
{
public:
    virtual ~IKLDebugUserNetworkingCommandContexInterface() = default;

    virtual void Serialize(FArchive& _Archive) = 0;
};
