// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.cpp"
#include "TestImgui/Hook.h"
DWORD Pulse();
BOOL APIENTRY DllMain(HMODULE Instance, DWORD  Reason, LPVOID lpReserved)
{

	if (Reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(Instance);
		Hook::hDDLModule = Instance;
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Pulse, nullptr, 0, nullptr);
	}
	return TRUE;
}

DWORD Pulse()
{
	*(int*)0x00D415B8 = 1;
	*(int*)0x00D415BC = 0x7FFFFFFF;
    Hook::HookDirectX();
    return 1;
}

