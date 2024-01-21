#pragma once

// engine
#include "GenericPlatform/GenericPlatform.h"

namespace KL::Debug::Server::Delegates
{
    //////////////////////////////////////////////////////
    /// classes

    class KLDEBUGUSERFRAMEWORK_API FServerSocketPortRangeDelegateData
    {
    public:
        explicit FServerSocketPortRangeDelegateData(const uint32 _StartRange, const uint32 _EndRange);

        UE_NODISCARD uint32 GetStartRange() const;
        UE_NODISCARD uint32 GetEndRange() const;

    private:
        uint32 mStartRange = 0;
        uint32 mEndRange = 0;
    };

    inline uint32 FServerSocketPortRangeDelegateData::GetStartRange() const
    {
        return mStartRange;
    }

    inline uint32 FServerSocketPortRangeDelegateData::GetEndRange() const
    {
        return mEndRange;
    }

}    // namespace KL::Debug::Server::Delegates