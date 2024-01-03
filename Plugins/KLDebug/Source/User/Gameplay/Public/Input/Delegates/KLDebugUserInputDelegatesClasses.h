// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

class UWorld;

namespace KL::Debug::User::Gameplay::Input
{
    class KLDEBUGUSERGAMEPLAY_API FOnInputTriggerData final
    {
    public:
        explicit FOnInputTriggerData(const UWorld& _World);

        UE_NODISCARD const UWorld& GetOwningWorld() const;

    private:
        const UWorld& mWorld;
    };

    inline const UWorld& FOnInputTriggerData::GetOwningWorld() const
    {
        return mWorld;
    }

}    // namespace KL::Debug::User::Gameplay::Input
