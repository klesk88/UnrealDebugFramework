// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "Luncher/Win/KLDebugNetworkingArbitrerLuncher_Win.h"

#if PLATFORM_WINDOWS

#include "Definitions/KLDebugNetworkingArbitrerDefinitions.h"
#include "Networking/Runtime/Public/Log/KLDebugNetworkingLog.h"

// engine
#include "Containers/UnrealString.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/HideWindowsPlatformTypes.h"

// c++
#include <Windows.h>

namespace KL::Debug::Networking::Arbitrer
{
    /////////////////////////////////////////////////////
    /// private

    static PROCESS_INFORMATION ProcessInfo{};

    /////////////////////////////////////////////////////
    /// public

    void LunchArbitrer(const FString& _ExecutablePath, const bool _CreateConsole)
    {
        DWORD HandleFlags = 0;
        if (GetHandleInformation(ProcessInfo.hProcess, &HandleFlags))
        {
            ensureMsgf(false, TEXT("we started already the arbitrer process"));
            // we already started the process
            return;
        }

        // based on TryAutoConnect in TraceAucilary.cpp

        // If we can detect a named event it means the arbitrer is already running.
        HANDLE KnownEvent = ::OpenEvent(EVENT_ALL_ACCESS, false, *KL::Debug::Networking::Arbitrer::ArbitrerSessionEvent);
        if (KnownEvent != nullptr)
        {
            UE_LOG(LogKLDebug_Networking, Display, TEXT("KL::Debug::Networking::Arbitrer::LunchArbitrer>> Arbitrer process already lunched"));
            return;
        }

        uint32 CreateProcFlags = CREATE_BREAKAWAY_FROM_JOB;
        if (_CreateConsole)
        {
            CreateProcFlags |= CREATE_NEW_CONSOLE;
        }
        else
        {
            CreateProcFlags |= CREATE_NO_WINDOW;
        }

        STARTUPINFOW StartupInfo = { sizeof(STARTUPINFOW) };
        const BOOL bOk = CreateProcessW(LPWSTR(*_ExecutablePath), nullptr, nullptr, nullptr, false, CreateProcFlags, nullptr, nullptr, &StartupInfo, &ProcessInfo);
        UE_CLOG(!bOk, LogKLDebug_Networking, Warning, TEXT("KL::Debug::Networking::Arbitrer::LunchArbitrerInternal>> unable to launch arbitrer with '%s' (%08x)"), *_ExecutablePath, GetLastError());
    }

    void CloseArbitrer()
    {
        DWORD HandleFlags = 0;
        if (!GetHandleInformation(ProcessInfo.hProcess, &HandleFlags))
        {
            // we didnt start any process nothing to close
            return;
        }

        TerminateProcess(ProcessInfo.hProcess, 0);

        // 500 ms timeout; use INFINITE for no timeout
        const DWORD Result = WaitForSingleObject(ProcessInfo.hProcess, 5000);
        if (Result == WAIT_OBJECT_0)
        {
            UE_LOG(LogKLDebug_Networking, Display, TEXT("KL::Debug::Networking::Arbitrer::CloseArbitrer>> Arbitrer process close"));
        }
        else
        {
            UE_LOG(LogKLDebug_Networking, Error, TEXT("KL::Debug::Networking::Arbitrer::CloseArbitrer>> Arbitrer process closure time out"));
        }

        CloseHandle(ProcessInfo.hProcess);
        CloseHandle(ProcessInfo.hThread);

        ProcessInfo = PROCESS_INFORMATION{};
    }

}    // namespace KL::Debug::Networking::Arbitrer

#endif