#include <Windows.h>

#include "Global.h"
#include "IPCServer.h"
#include "pipe_sender.h"

#include "msg_hook.h"

FILE* log_out;
HANDLE h_pipe = (HANDLE)-1;

HANDLE hProcess = (HANDLE)-1;

namespace msg_hook {
    DWORD insert_chatlog_addr;
    DWORD call_proc_addr;
    DWORD hook_orig_addr;
    BYTE orig_bytes[5];
}

namespace pipe_sender {
    queue<message_data> send_queue;
    HANDLE process_thread = NULL;
}

BOOL IsElevated() {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if (hToken) {
        CloseHandle(hToken);
    }
    return fRet;
}

int main_t(HMODULE hModule) {
    if (!IsElevated()) {
        MessageBoxA(NULL, "카카오톡을 관리자 권한으로 실행해주세요.", "Kakaotalk Module", MB_OK | MB_ICONERROR);
        FreeLibraryAndExitThread(hModule, 0);
        return -1;
    }

#ifdef DEBUG_MODE
        log_out = fopen("C:\\_kakaotalk_module.log", "w");
        setbuf(log_out, NULL);
#endif

    write_log("Module Loaded!\n");

    if (!msg_hook::init()) {
        MessageBoxA(NULL, "초기화 도중 오류가 발생하였습니다.", "Kakaotalk Module", MB_OK | MB_ICONERROR);
        FreeLibraryAndExitThread(hModule, 0);
        return -1;
    }

    h_pipe = CreateMapHandle(PIPE_NAME);
    if ((DWORD)h_pipe < 0) {
        MessageBoxA(NULL, "초기화 도중 오류가 발생하였습니다.", "Kakaotalk Module", MB_OK | MB_ICONERROR);
        FreeLibraryAndExitThread(hModule, 0);
        return -1;
    }
    write_log("[Pipe Handle] %p\n", h_pipe);

    pipe_sender::init();

    MessageBoxA(NULL, "Press OK to Unload Module.", "Kakaotalk Module", MB_OK | MB_ICONINFORMATION);

    write_log("Module Unloaded!\n");
#ifdef DEBUG_MODE
    fclose(log_out);
#endif

    msg_hook::destroy();
    pipe_sender::destroy();
    CloseHandle(h_pipe);

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DWORD thread_id;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main_t, (LPVOID)hModule, 0, &thread_id);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

