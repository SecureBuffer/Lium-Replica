#include "Drawing.h"
#include "Hook.h"
#include "../lua.h"

BOOL Drawing::bInit = FALSE; // Status of the initialization of ImGui.
bool Drawing::bDisplay = true; // Status of the menu display.
bool Drawing::bSetPos = false; // Status to update ImGui window size / position.
ImVec2 Drawing::vWindowPos = { 0, 0 }; // Last ImGui window position.
ImVec2 Drawing::vWindowSize = { 0, 0 }; // Last ImGui window size.

/**
	@brief : Hook of the EndScene function.
	@param  D3D9Device : Current Direct3D9 Device Object.
	@retval : Result of the original EndScene function.
**/

char _Buffer[64] = "CustomFunction";
HRESULT Drawing::hkEndScene(const LPDIRECT3DDEVICE9 D3D9Device)
{
	if (!Hook::pDevice)
		Hook::pDevice = D3D9Device;

	if (!bInit)
		InitImGui(D3D9Device);

	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDisplay = !bDisplay;

	if (GetAsyncKeyState(VK_END) & 1)
	{
		Hook::UnHookDirectX();
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, Hook::hDDLModule, 0, nullptr);
		return Hook::oEndScene(D3D9Device);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (bDisplay)
	{
		ImGui::SetNextWindowSize({ (float)500, (float)450 }, ImGuiCond_Once);
		if (ImGui::Begin("Lium", &bDisplay))
		{
			ImGui::Text("FunctionName");
			ImGui::SameLine();
			ImGui::InputText(" ", _Buffer, IM_ARRAYSIZE(_Buffer));
			if (ImGui::Button("Register Function"))
			{
				lua_Register(_Buffer, (int)RunProtectedFunction);
			}
		}
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return Hook::oEndScene(D3D9Device);
}

/**
	@brief : function that init ImGui for rendering.
	@param pDevice : Current Direct3D9 Device Object given by the hooked function.
**/
void Drawing::InitImGui(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DDEVICE_CREATION_PARAMETERS CP;
	pDevice->GetCreationParameters(&CP);
	Hook::window = CP.hFocusWindow;
	Hook::HookWindow();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;


	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 30.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

	// Set ImGui configuration flags
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigFlags |= ImGuiConfigFlags_NavNoCaptureKeyboard;



	ImGui_ImplWin32_Init(Hook::window);
	ImGui_ImplDX9_Init(pDevice);

	bInit = TRUE;
}
