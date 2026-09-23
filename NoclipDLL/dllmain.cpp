#include <iostream>
#include <windows.h>
#include <MinHook.h>

// global variable for noclip
// auto disabled
bool noclipEnabled = false;

// declare the function and pointer
typedef void(*destroyPlayer_t)(void* thisPtr, void* player, void* object);
destroyPlayer_t original_destroyPlayer = nullptr;

void hooked_destroyPlayer(void* thisPtr, void* player, void* object) {
    if (noclipEnabled) {
        // if noclip is enabled don't do anything
        return;
    }
    original_destroyPlayer(thisPtr, player, object);
}

DWORD WINAPI KeyListenerThread(LPVOID lpParam) {
    bool wasPressed = false;

    while (true) {
        bool isPressed = (GetAsyncKeyState(VK_RETURN) & 0x8000) != 0;

        // toggle on the moment it goes from pressed to not pressed or else it lags
        if (isPressed && !wasPressed) {
            noclipEnabled = !noclipEnabled;
        }

        wasPressed = isPressed;
        Sleep(50); // poll 20 times per second because if it goes to fast you crash
    }
    return 0;
}

DWORD WINAPI MainThread(LPVOID lpParam) {
    // make a console for logs
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);

    // initialize
    MH_Initialize();

    std::cout << "[DLL] Noclip loaded!" << std::endl;
    std::cout << "[DLL] Press enter to toggle." << std::endl;
    std::cout << " =^..^= " << std::endl;

    uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
    uintptr_t targetAddress = base + 0x3b39d0;

    MH_CreateHook((LPVOID)targetAddress, &hooked_destroyPlayer, (LPVOID*)&original_destroyPlayer);
    MH_EnableHook((LPVOID)targetAddress);

    CreateThread(nullptr, 0, KeyListenerThread, nullptr, 0, nullptr);

    return 0;
}

// entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, MainThread, nullptr, 0, nullptr);
        break;
    }
    return TRUE;
}