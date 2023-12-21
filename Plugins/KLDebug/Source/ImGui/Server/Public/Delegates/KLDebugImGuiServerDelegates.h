#pragma once

// engine
#include "Delegates/DelegateCombinations.h"
#include "GenericPlatform/GenericPlatform.h"
#include "Misc/Optional.h"

namespace KL::Debug::Server::Delegates
{
    //////////////////////////////////////////////////////
    /// classes

    class KLDEBUGIMGUISERVER_API FServerSocketPortRangeDelegateData
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

    //////////////////////////////////////////////////////
    /// methods

    DECLARE_DELEGATE_RetVal(FServerSocketPortRangeDelegateData, FOnGetDebugServerSocketPortRange);

    KLDEBUGIMGUISERVER_API void BindOnGetDebugServerSocketPortRange(const FOnGetDebugServerSocketPortRange& _Delegate);
    KLDEBUGIMGUISERVER_API void UnbindOnGetDebugServerSocketPortRange();
    // without _API on purpose as it should be called just internally this module
    UE_NODISCARD TOptional<FServerSocketPortRangeDelegateData> BroadcastOnGetDebugServerSocketPortRange();

}    // namespace KL::Debug::Server::Delegates