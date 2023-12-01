// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "Message/Header/KLDebugImGuiNetworkingMessage_Header.h"

// engine
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "HAL/Platform.h"

class KLDEBUGIMGUINETWORKING_API FKLDebugImGuiNetworkingMessageContainer
{
public:
    void Serialize(FArchive& _Archive);

private:
    FKLDebugImGuiNetworkingMessage_Header mMessageHeader;
    TArray<uint8> mMessageSerializedData;
};
