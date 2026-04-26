#include <windows.h>

namespace ServiceImpl {
    SERVICE_STATUS status{};
    SERVICE_STATUS_HANDLE statusHandle = nullptr;

    const wchar_t SERVICE_NAME[] = L"Tema3HelloService";

    bool running = true;

    void SetState(DWORD newState, DWORD error = NO_ERROR, DWORD hint = 0) {
        status.dwCurrentState = newState;
        status.dwWin32ExitCode = error;
        status.dwWaitHint = hint;
        SetServiceStatus(statusHandle, &status);
    }

    void WriteLog(const wchar_t* message) {
        HANDLE h = RegisterEventSourceW(nullptr, SERVICE_NAME);
        if (!h) return;

        LPCWSTR msgs[] = { message };

        ReportEventW(
            h,
            EVENTLOG_INFORMATION_TYPE,
            0,
            0x1000,
            nullptr,
            1,
            0,
            msgs,
            nullptr
        );

        DeregisterEventSource(h);
    }

    void WINAPI ControlHandler(DWORD code) {
        switch (code) {
            case SERVICE_CONTROL_STOP:
                running = false;
                SetState(SERVICE_STOP_PENDING, NO_ERROR, 1000);
                break;
            default:
                break;
        }
    }

    void WINAPI ServiceMain(DWORD, LPWSTR*) {
        statusHandle = RegisterServiceCtrlHandlerW(SERVICE_NAME, ControlHandler);
        if (!statusHandle) return;
        status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
        status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
        status.dwWin32ExitCode = NO_ERROR;
        status.dwServiceSpecificExitCode = 0;
        status.dwCheckPoint = 0;

        SetState(SERVICE_START_PENDING);

        SetState(SERVICE_RUNNING);
        WriteLog(L"Hello World!");

        while (running) {
            Sleep(500);
        }

        SetState(SERVICE_STOPPED);
    }
}

int wmain() {
    SERVICE_TABLE_ENTRYW table[] = {
        { const_cast<LPWSTR>(ServiceImpl::SERVICE_NAME), ServiceImpl::ServiceMain },
        { nullptr, nullptr }
    };

    if (!StartServiceCtrlDispatcherW(table)) {
        ServiceImpl::WriteLog(L"Not started as service.");
    }

    return 0;
}