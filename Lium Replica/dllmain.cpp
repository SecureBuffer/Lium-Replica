// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.cpp"
#include "variableheader.h"
int endScenePointer{};
DWORD Pulse();
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Pulse, NULL, 0, NULL);
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            shouldRemoveEndSceneInjection = true;
            break;
    }
    return TRUE;
}

VOID FunctionPointerFix()
{
    *(int*)0x00D415B8 = 1;
    *(int*)0x00D415BC = 0x7FFFFFFF;


    UINT pDevice = *(UINT*)0xC5DF88;
    UINT pEnd = *(UINT*)(pDevice + 0x397C);
    UINT Pscene = *(UINT*)pEnd;
    UINT PEndScene = *(UINT*)(Pscene + 0xA8);
    endScenePointer = PEndScene;
}

DWORD Pulse()
{
    FunctionPointerFix();
    DetourHooks["EndScene"] = new Detour(endScenePointer, (int)EndScene);
    return 1;
}

