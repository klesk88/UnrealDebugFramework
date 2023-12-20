// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Luncher/KLDebugNetworkingArbitrerLuncher.h"

#include "Log/KLDebugNetworkingArbitrerLog.h"

#if PLATFORM_WINDOWS
#include "Luncher/Win/KLDebugNetworkingArbitrerLuncher_Win.h"
#elif PLATFORM_UNIX
#include "Luncher/Linux/KLDebugNetworkingArbitrerLuncher_Linux.h"
#endif

// engine
#include "Containers/UnrealString.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Templates/SharedPointer.h"

namespace KL::Debug::Networking::Arbitrer
{
    /////////////////////////////////////////////////////
    /// private
    FOnArbitrerShouldLunch OnArbitrerShouldLunch;

    static bool HasStartArbitrer = false;

    /////////////////////////////////////////////////////
    /// public

    void BindOnArbitrerShouldLunch(const FOnArbitrerShouldLunch& _Delegate)
    {
        ensureMsgf(!OnArbitrerShouldLunch.IsBound(), TEXT("delegate is already bound"));

        OnArbitrerShouldLunch = _Delegate;
    }

    void UnbindOnArbitrerShouldLunch()
    {
        OnArbitrerShouldLunch.Unbind();
    }

    void TryLunchArbitrer()
    {
        static const FString ExecutablePath = FString::Format(TEXT("{0}/Resources/Arbitrer/KLDebugArbitrer.exe"), { *(IPluginManager::Get().FindPlugin("KLDebug")->GetBaseDir()) });
        if (HasStartArbitrer)
        {
            return;
        }

        HasStartArbitrer = true;
        if (OnArbitrerShouldLunch.IsBound() && !OnArbitrerShouldLunch.Execute())
        {
            UE_LOG(LogKL_DebugArbitrer, Display, TEXT("KL::Debug::Networking::Arbitrer::TryLunchArbitrer>> Arbitrer will not lunch as delegate doesn't allow so"));
            return;
        }

        const FString CmdLine = FCommandLine::Get();
        const bool CreateConsole = FParse::Param(*CmdLine, TEXT("showarbitrerlog"));

        KL::Debug::Networking::Arbitrer::LunchArbitrer(ExecutablePath, CreateConsole);
    }

    void TryCloseArbitrer()
    {
        KL::Debug::Networking::Arbitrer::CloseArbitrer();
        HasStartArbitrer = false;
    }

    bool IsArbitrerRunning()
    {
        return KL::Debug::Networking::Arbitrer::IsArbitrerRunningInternal();
    }

}    // namespace KL::Debug::Networking::Arbitrer
