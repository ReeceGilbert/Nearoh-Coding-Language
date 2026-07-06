#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "process.h"

#include <stdio.h>
#include <string.h>

int process_run_capture(const char* command_line, const char* working_directory, char* output, int output_capacity) {
    SECURITY_ATTRIBUTES security;
    HANDLE read_pipe;
    HANDLE write_pipe;
    STARTUPINFOA startup;
    PROCESS_INFORMATION process;
    char command_copy[2048];
    DWORD bytes_read;
    DWORD total_read;
    DWORD exit_code;

    if (output == NULL || output_capacity <= 0) {
        return -1;
    }

    output[0] = '\0';

    security.nLength = sizeof(SECURITY_ATTRIBUTES);
    security.bInheritHandle = TRUE;
    security.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&read_pipe, &write_pipe, &security, 0)) {
        lstrcpynA(output, "Failed to create pipe.\n", output_capacity);
        return -1;
    }

    SetHandleInformation(read_pipe, HANDLE_FLAG_INHERIT, 0);

    ZeroMemory(&startup, sizeof(startup));
    ZeroMemory(&process, sizeof(process));

    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdOutput = write_pipe;
    startup.hStdError = write_pipe;
    startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    lstrcpynA(command_copy, command_line, sizeof(command_copy));

    if (!CreateProcessA(
        NULL,
        command_copy,
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL,
        working_directory,
        &startup,
        &process
    )) {
        DWORD error = GetLastError();
        snprintf(output, (size_t)output_capacity, "Failed to run process. Windows error: %lu\n", error);
        CloseHandle(read_pipe);
        CloseHandle(write_pipe);
        return -1;
    }

    CloseHandle(write_pipe);

    total_read = 0;

    while (ReadFile(
        read_pipe,
        output + total_read,
        (DWORD)(output_capacity - total_read - 1),
        &bytes_read,
        NULL
    ) && bytes_read > 0) {
        total_read += bytes_read;

        if (total_read >= (DWORD)(output_capacity - 1)) {
            break;
        }
    }

    output[total_read] = '\0';

    WaitForSingleObject(process.hProcess, INFINITE);
    GetExitCodeProcess(process.hProcess, &exit_code);

    CloseHandle(read_pipe);
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);

    return (int)exit_code;
}