#pragma once
#include "pch.cpp"


HWND ImGuiwindow = nullptr;
WNDPROC oWndProc = nullptr;
bool init = false;
bool openmenu = true;


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT _stdcall WndProc(const HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitImGui(LPDIRECT3DDEVICE9 pDevice);
LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam);

class Graphics
{
private:

   
    HHOOK keyboardHook;
    ImGuiWindowFlags window_flags = ImGuiNavInput_Cancel;
    

    const DWORD line_fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;

public:

    Graphics();
    ~Graphics();
    void Pulse(LPDIRECT3DDEVICE9 device);
};