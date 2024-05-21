// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

// engine
#include "Containers/Array.h"
#include "Delegates/DelegateCombinations.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "HAL/Platform.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/NameTypes.h"

class APlayerController;
class UWorld;

namespace KL::Debug::Networking::Commands::Internal
{
    using DebugCommandClientFilter = TFunction<bool(const APlayerController&)>;

    class KLDEBUGUSERFRAMEWORK_API FKLDebugOnNewCommandInput final : public FNoncopyable
    {
    public:
        explicit FKLDebugOnNewCommandInput(const UWorld& _World, const FName& _CommandID, const DebugCommandClientFilter& _ClientFilter, TArray<uint8>&& _ContextData);

        [[nodiscard]] const UWorld& GetWorld() const;
        [[nodiscard]] const FName& GetCommandID() const;
        [[nodiscard]] const DebugCommandClientFilter& GetClientFilter() const;
        [[nodiscard]] TArray<uint8>&& GetContextDataMutable() const;
        [[nodiscard]] const TArray<uint8>& GetContextData() const;

    private:
        const UWorld& mWorld;
        const FName& mCommandID;
        const DebugCommandClientFilter& mClientFilter;
        TArray<uint8>&& mContextData;
    };

    inline const UWorld& FKLDebugOnNewCommandInput::GetWorld() const
    {
        return mWorld;
    }

    inline const FName& FKLDebugOnNewCommandInput::GetCommandID() const
    {
        return mCommandID;
    }

    inline const DebugCommandClientFilter& FKLDebugOnNewCommandInput::GetClientFilter() const
    {
        return mClientFilter;
    }

    inline TArray<uint8>&& FKLDebugOnNewCommandInput::GetContextDataMutable() const
    {
        return MoveTemp(mContextData);
    }

    inline const TArray<uint8>& FKLDebugOnNewCommandInput::GetContextData() const
    {
        return mContextData;
    }

}    // namespace KL::Debug::Networking::Commands::Internal