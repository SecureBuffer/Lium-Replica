#include "pch.cpp"
#include "variableheader.h"

Graphics* graph = nullptr;


int __stdcall EndScene(LPDIRECT3DDEVICE9 device)
{
	if (graph == nullptr && device)
		graph = new Graphics();

	if (openmenu)
		graph->Pulse(device);



	#pragma region Restore Detour
		auto det = DetourHooks["EndScene"];
		det->Restore();
		int res = ((int(__stdcall*)(LPDIRECT3DDEVICE9))det->target)(device);
		if (shouldRemoveEndSceneInjection)
		{

			auto it = DetourHooks.find("EndScene");
			delete it->second;
			DetourHooks.erase(it);

			if (graph != nullptr)
			{
				graph->~Graphics();
				delete graph;
			}
		}
		else
		{
			det->Apply();
		}
		return res;
	#pragma endregion
}