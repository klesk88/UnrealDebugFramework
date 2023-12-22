// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Luncher/KLDebugNetworkingArbitrerLuncher.h"

#include "Log/KLDebugNetworkingArbitrerLog.h"

#if PLATFORM_WINDOWS
#include "Luncher/Win/KLDebugNetworkingArbitrerLuncher_Win.h"
#elif PLATFORM_UNIX
#include "Luncher/Linux/KLDebugNetworkingArbitrerLuncher_Linux.h"
#endif

// engine
#include "Interfaces/IPluginManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Paths.h"
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

    bool IsArbitrerSupported()
    {
        if (OnArbitrerShouldLunch.IsBound())
        {
            return OnArbitrerShouldLunch.Execute();
        }

        return true;
    }

    bool TryLunchArbitrer()
    {
        static const FString AbsSavePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir());
        static const FString ExecutablePath = FString::Format(TEXT("{0}/Resources/Arbitrer"), { *(IPluginManager::Get().FindPlugin("KLDebug")->GetBaseDir()) });
        // Add -UserDir here so we can override the location of the logs for the arbitrer and have them in the project Saved Folder (like all the rest)
        // instead of some arbitrary folder under the Plugin folder
        static const FString ExecutablePathWithExe = FString::Format(TEXT("{0}/KLDebugArbitrer.exe -UserDir={1}/DebugArbitrer"), { *ExecutablePath, *AbsSavePath });

        if (HasStartArbitrer)
        {
            return false;
        }

        HasStartArbitrer = true;
        if (!IsArbitrerSupported())
        {
            UE_LOG(LogKL_DebugArbitrer, Display, TEXT("KL::Debug::Networking::Arbitrer::TryLunchArbitrer>> Arbitrer will not lunch as delegate doesn't allow so"));
            return false;
        }

        const FString CmdLine = FCommandLine::Get();
        const bool CreateConsole = FParse::Param(*CmdLine, TEXT("showarbitrerlog"));

        return KL::Debug::Networking::Arbitrer::LunchArbitrer(ExecutablePathWithExe, CreateConsole);
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
