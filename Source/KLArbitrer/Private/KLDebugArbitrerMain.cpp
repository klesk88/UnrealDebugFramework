// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugArbitrer_Server.h"

// modules
#include "Networking/Arbitrer/Public/Definitions/KLDebugNetworkingArbitrerDefinitions.h"
#include "Networking/Arbitrer/Public/Log/KLDebugNetworkingArbitrerLog.h"
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"

// engine
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "HAL/RunnableThread.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "RequiredProgramMainCPPInclude.h"

IMPLEMENT_APPLICATION(DebugArbitrer, "DebugArbitrer");

namespace KL::Arbitrer
{
    UE_NODISCARD bool CheckArbitrerSingleInstance()
    {
        // based on CheckSessionBrowserSingleInstance in UserInterfaceCommand.cpp

#if PLATFORM_WINDOWS
        HANDLE ArbitrerEvent = CreateEvent(nullptr, true, false, *KL::Debug::Networking::Arbitrer::ArbitrerSessionEvent);
        if (ArbitrerEvent == nullptr || GetLastError() == ERROR_ALREADY_EXISTS)
        {
            if (ArbitrerEvent != nullptr)
            {
                CloseHandle(ArbitrerEvent);
            }

            // Activate the respective window.
            HWND Window = FindWindowW(0, L"KLDebugArbitrer");
            if (Window)
            {
                ShowWindow(Window, SW_SHOW);
                SetForegroundWindow(Window);

                FLASHWINFO FlashInfo;
                FlashInfo.cbSize = sizeof(FLASHWINFO);
                FlashInfo.hwnd = Window;
                FlashInfo.dwFlags = FLASHW_ALL;
                FlashInfo.uCount = 3;
                FlashInfo.dwTimeout = 0;
                FlashWindowEx(&FlashInfo);
            }

            return false;
        }
#elif PLATFORM_UNIX
        int FileHandle = open("/var/run/KLDebugArbitrer.pid", O_CREAT | O_RDWR, 0666);
        int Ret = flock(FileHandle, LOCK_EX | LOCK_NB);
        if (Ret && EWOULDBLOCK == errno)
        {
            return false;
        }
#endif

        return true;
    }

    UE_NODISCARD FString GetLogFolder()
    {
        FString CmdLine(::GetCommandLineW());
        if (CmdLine.IsEmpty())
        {
            return FString();
        }

        if (CmdLine[0] == '"')
        {
            CmdLine.RemoveAt(0, 1, false);
        }

        int32 StringIndex = CmdLine.Find(TEXT("Binaries"));
        if (StringIndex == INDEX_NONE)
        {
            return FString();
        }

        CmdLine.RemoveAt(StringIndex, static_cast<int32>(CmdLine.Len()) - StringIndex, false);
        CmdLine.ReplaceInline(TEXT("\\"), TEXT("/"), ESearchCase::CaseSensitive);
        CmdLine.Append(TEXT("Saved/"));
        return CmdLine;
    }
}    // namespace KL::Arbitrer

// based on UnrealInsightsMain and BlankProject
INT32_MAIN_INT32_ARGC_TCHAR_ARGV()
{
    FTaskTagScope Scope(ETaskTag::EGameThread);
    ON_SCOPE_EXIT
    {
        LLM(FLowLevelMemTracker::Get().UpdateStatsPerFrame());
        RequestEngineExit(TEXT("Exiting"));
        FEngineLoop::AppPreExit();
        FModuleManager::Get().UnloadModulesAtShutdown();
        FEngineLoop::AppExit();
    };

    const FString InitialCmdLine(::GetCommandLineW());
    FString ArbitrerWorkingDir;
    FString CommandLine(::GetCommandLineW());
    if (!FParse::Value(*CommandLine, TEXT("-UserDir="), ArbitrerWorkingDir))
    {
        // this should mean we are building with VS and running trough it. So set the folder
        // ourself.
        // When the server spins up the arbiter instead, we expect the server to add this cmd line
        // so it points to the right folder
        const FString LogFolder = KL::Arbitrer::GetLogFolder();
        ArbitrerWorkingDir = *LogFolder;
        CommandLine = FString::Format(TEXT("{0} -UserDir={1}DebugArbitrer"), { ::GetCommandLineW(), *ArbitrerWorkingDir });
    }

    if (int32 Ret = GEngineLoop.PreInit(*CommandLine))
    {
        return Ret;
    }

    if (!KL::Arbitrer::CheckArbitrerSingleInstance())
    {
        return 0;
    }

    // Tell the module manager it may now process newly-loaded UObjects when new C++ modules are loaded.
    FModuleManager::Get().StartProcessingNewlyLoadedObjects();

    // based on FUserInterfaceCommand::Run

    IPluginManager::Get().LoadModulesForEnabledPlugins(ELoadingPhase::PreDefault);
    IPluginManager::Get().LoadModulesForEnabledPlugins(ELoadingPhase::Default);
    FModuleManager::Get().LoadModuleChecked("Networking");

    FKLDebugNetworkArbitrer_Server Server;
    if (!Server.Init())
    {
        return 0;
    }

    KL::Debug::Networking::Message::Init(1024, 1024, 0, 0);

    UE_LOG(LogKL_DebugArbitrer, Display, TEXT("Arbitrer log dir set at [%s]"), *ArbitrerWorkingDir);
    static constexpr float SecondsToSleep = 0.2f;
    while (true)
    {
        Server.Run();
        FPlatformProcess::Sleep(SecondsToSleep);
    }

    Server.Exit();
    return 0;
}