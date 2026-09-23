#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>
#include <string>

// turn process name into pid
int findGDProcess() {

    HANDLE hSnapshot;
    PROCESSENTRY32 pe;
    int pid = 0;
    BOOL hResult;

    // take a snapshot of processes
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot) return 0;

    pe.dwSize = sizeof(PROCESSENTRY32);

    hResult = Process32First(hSnapshot, &pe);

    // ok get gd's pid
    while (hResult) {
        if (wcscmp(pe.szExeFile, L"GeometryDash.exe") == 0) {
            pid = pe.th32ProcessID;
            break;
        }
        hResult = Process32Next(hSnapshot, &pe);
    }

    CloseHandle(hSnapshot);
    return pid;
}

bool Inject(DWORD pid, const char* dllPath)
{
    HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (h)
    {
        LPVOID LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
        size_t pathLen = strlen(dllPath) + 1;
        LPVOID dereercomp = VirtualAllocEx(h, NULL, pathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(h, dereercomp, dllPath, pathLen, NULL);
        HANDLE asdc = CreateRemoteThread(h, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddr, dereercomp, 0, NULL);
        WaitForSingleObject(asdc, INFINITE);
        VirtualFreeEx(h, dereercomp, pathLen, MEM_RELEASE);
        CloseHandle(asdc);
        CloseHandle(h);
        return true;
    }
    return false;
}

// get full path of dll so i don't have to hardcode it or put it in the gd folder
// WHY DO I HAVE TO DO THIS IN C++
std::string GetDllPathNextToExe(const char* dllName)
{
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    std::string path(exePath);
    size_t lastSlash = path.find_last_of("\\/");
    if (lastSlash != std::string::npos) {
        path = path.substr(0, lastSlash + 1); // keep trailing slash
    }

    path += dllName;
    return path;
}

int main()
{
    int pid = findGDProcess();

    // make sure gd is open
    if (pid == 0) {
        std::cout << "[injector] gd pid not found. make sure gd is open!" << std::endl;
        return 1;
    }

    std::cout << "[injector] gd pid found : " << pid << std::endl;

    // get the path of the dll
    std::string dllPath = GetDllPathNextToExe("NoclipDLL.dll");
    std::cout << "[injector] using dll path : " << dllPath << std::endl;

    // inject
    Inject(pid, dllPath.c_str());
}