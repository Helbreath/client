
#include "include/base/cef_build.h"
#include "include/cef_app.h"

#include <windows.h>
#include <tchar.h>

enum ProcessType {
    PROCESS_TYPE_BROWSER,
    PROCESS_TYPE_RENDERER,
    PROCESS_TYPE_OTHER,
};

CefRefPtr<CefApp> CreateRendererProcessApp();
CefRefPtr<CefCommandLine> CreateCommandLine(const CefMainArgs & main_args);
ProcessType GetProcessType(const CefRefPtr<CefCommandLine> & command_line);

// These flags must match the Chromium values.
const char kProcessType[] = "type";
const char kRendererProcess[] = "renderer";
#if defined(OS_LINUX)
const char kZygoteProcess[] = "zygote";
#endif

CefRefPtr<CefCommandLine> CreateCommandLine(const CefMainArgs & main_args) {
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
#if defined(OS_WIN)
    command_line->InitFromString(::GetCommandLineW());
#else
    command_line->InitFromArgv(main_args.argc, main_args.argv);
#endif
    return command_line;
}

ProcessType GetProcessType(const CefRefPtr<CefCommandLine> & command_line) {
    // The command-line flag won't be specified for the browser process.
    if (!command_line->HasSwitch(kProcessType))
        return PROCESS_TYPE_BROWSER;

    const std::string & process_type = command_line->GetSwitchValue(kProcessType);
    if (process_type == kRendererProcess)
        return PROCESS_TYPE_RENDERER;

#if defined(OS_LINUX)
    // On Linux the zygote process is used to spawn other process types. Since we
    // don't know what type of process it will be we give it the renderer app.
    if (process_type == kZygoteProcess)
        return PROCESS_TYPE_RENDERER;
#endif

    return PROCESS_TYPE_OTHER;
}


#ifdef _DEBUG
//////////////////////////////////////////////////////////////////////////
// debug named pipes
#define BUFSIZE 2048
HANDLE hPipe;
LPTSTR lpvMessage = TEXT("Default message from client.");
TCHAR  chBuf[BUFSIZE];
BOOL   fSuccess = FALSE;
DWORD  cbRead, cbToWrite, cbWritten, dwMode;
LPSTR lpszPipename = "\\\\.\\pipe\\stupidcefdebuglog";
//////////////////////////////////////////////////////////////////////////
#endif

std::string processtype;

void send_pipe_message(std::string msg)
{
#ifndef _DEBUG
    return;
#else
    std::string tosend = processtype + msg;
    fSuccess = WriteFile(
        hPipe,                  // pipe handle 
        tosend.c_str(),            // message 
        tosend.length()+1,         // message length 
        &cbWritten,             // bytes written 
        NULL);                  // not overlapped 
#endif
}

// Entry point function for all processes.
int main(int argc, char * argv[]) {
//int APIENTRY wWinMain(HINSTANCE hInstance) {
    // Enable High-DPI support on Windows 7 or newer.
    CefEnableHighDPISupport();

#ifdef _DEBUG
    //////////////////////////////////////////////////////////////////////////
    // debug named pipes
    while (1)
    {
        hPipe = CreateFileA(
            lpszPipename,   // pipe name 
            GENERIC_READ |  // read and write access 
            GENERIC_WRITE,
            0,              // no sharing 
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe 
            0,              // default attributes 
            NULL);          // no template file 

      // Break if the pipe handle is valid. 

        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        // Exit if an error other than ERROR_PIPE_BUSY occurs. 

        if (GetLastError() != ERROR_PIPE_BUSY)
        {
            _tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
            return -1;
        }

        // All pipe instances are busy, so wait for 20 seconds. 

        if (!WaitNamedPipeA(lpszPipename, 20000))
        {
            printf("Could not open pipe: 20 second wait timed out.");
            return -1;
        }
    }

    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(
        hPipe,    // pipe handle 
        &dwMode,  // new pipe mode 
        NULL,     // don't set maximum bytes 
        NULL);    // don't set maximum time 
    if (!fSuccess)
    {
        _tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
        return -1;
    }
#endif

    CefMainArgs main_args(GetModuleHandle(NULL));

    CefRefPtr<CefCommandLine> command_line = CreateCommandLine(main_args);

    CefRefPtr<CefApp> app;
    switch (GetProcessType(command_line)) {
        case PROCESS_TYPE_RENDERER:
            app = CreateRendererProcessApp();
            processtype = "<Renderer> ";
            break;
    }

    send_pipe_message("About to execute process");

    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0) {
        return exit_code;
    }
    return 0;
}
