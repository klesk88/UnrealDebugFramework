// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Server/KLDebugArbitrer_Server.h"

// modules
#include "Networking/Runtime/Public/Message/Helpers/KLDebugNetworkingMessageHelpers.h"

// engine
#include "CoreMinimal.h"
#include "HAL/RunnableThread.h"
#include "Interfaces/IPluginManager.h"
#include "RequiredProgramMainCPPInclude.h"

IMPLEMENT_APPLICATION(KLDebugFramework, "KLDebugFramework");

// based on CheckSessionBrowserSingleInstance in FUserInterfaceCommand
UE_NODISCARD bool CheckSessionBrowserSingleInstance()
{
#if PLATFORM_WINDOWS
    // Create a named event that other processes can detect.
    // It allows only a single instance of Unreal Insights (Browser Mode).
    // The event is also used by runtime to choose when to try to auto-connect.
    // See FTraceAuxiliary::TryAutoConnect() in \Runtime\Core\Private\ProfilingDebugging\TraceAuxiliary.cpp
    HANDLE SessionBrowserEvent = CreateEvent(NULL, true, false, *KL::Debug::Networking::Message::ArbitrerSessionEvent);
    if (SessionBrowserEvent == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
    {
        // Another Session Browser process is already running.

        if (SessionBrowserEvent != NULL)
        {
            CloseHandle(SessionBrowserEvent);
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
#endif    // PLATFORM_WINDOWS

#if PLATFORM_UNIX
    int FileHandle = open("/var/run/KLDebugArbitrer.pid", O_CREAT | O_RDWR, 0666);
    int Ret = flock(FileHandle, LOCK_EX | LOCK_NB);
    if (Ret && EWOULDBLOCK == errno)
    {
        // Another Session Browser process is already running.

        // Activate the respective window.
        // TODO: "wmctrl -a Insights"

        return false;
    }
#endif

    return true;
}

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

    if (int32 Ret = GEngineLoop.PreInit(ArgC, ArgV))
    {
        return Ret;
    }

    if (!CheckSessionBrowserSingleInstance())
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

    KL::Debug::Networking::Message::InitHeaderSize();

    static constexpr float SecondsToSleep = 0.2f;
    while (true)
    {
        Server.Run();
        FPlatformProcess::Sleep(SecondsToSleep);
    }

    Server.Exit();
    return 0;
}