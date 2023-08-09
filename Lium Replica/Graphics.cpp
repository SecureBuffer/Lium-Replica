#include "Graphics.h"
#include "lua.h"

Graphics::Graphics()
{
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, NULL, 0);
}
Graphics::~Graphics()
{
 
    SetWindowLongPtr(ImGuiwindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));
    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX9_Shutdown();
    ImGui::DestroyContext();
    UnhookWindowsHookEx(keyboardHook);
   
}
void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
    if (!init)
    {
        D3DDEVICE_CREATION_PARAMETERS CP;
        pDevice->GetCreationParameters(&CP);
        ImGuiwindow = CP.hFocusWindow;

        // Store the original WndProc for later use
        oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(ImGuiwindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        io.IniFilename = nullptr;

        // Load fonts and set font size (assuming Arial.ttf is in the specified location)
        io.Fonts->AddFontDefault();
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 35.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());

        // Set ImGui configuration flags
        //io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        //io.ConfigFlags |= ImGuiConfigFlags_NavNoCaptureKeyboard;

        // Initialize ImGui Win32 and DirectX9 bindings
        ImGui_ImplWin32_Init(ImGuiwindow);
        ImGui_ImplDX9_Init(pDevice);
        init = true;
    }
}
LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
    {
        KBDLLHOOKSTRUCT* kbData = (KBDLLHOOKSTRUCT*)lParam;

        // Check for VK_F5 key press to toggle openmenu variable
        if (kbData->vkCode == VK_F5)
        {
            openmenu = !openmenu;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Process ImGui input events
    if (openmenu && ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;

    // Update ImGui mouse position
    ImGuiIO& io = ImGui::GetIO();
    POINT mPos;
    GetCursorPos(&mPos);
    ScreenToClient(hwnd, &mPos);
    io.MousePos.x = static_cast<float>(mPos.x);
    io.MousePos.y = static_cast<float>(mPos.y);


    switch (uMsg)
    {
        case WM_SIZE:
            if (wParam == SIZE_MAXIMIZED || wParam == SIZE_MINIMIZED || wParam == SIZE_RESTORED)
            {
                if (ImGuiwindow != NULL) 
                {
                    ImGui_ImplDX9_InvalidateDeviceObjects();
               
                }
            }
            break;
    }

    if (openmenu)
    {
       
        if (io.WantCaptureKeyboard || io.WantCaptureMouse)
            return 1l;
    }

    return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
}



char Buffer[64] = "CustomFunction";

void Graphics::Pulse(LPDIRECT3DDEVICE9 device)
{
    InitImGui(device);
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();


    if (openmenu)
    {
        ImGui::SetNextWindowSize({ (float)500, (float)450 });
        if (ImGui::Begin("Lium", &openmenu))
        {
            ImGui::Text("FunctionName");
            ImGui::SameLine();
            ImGui::InputText(" ", Buffer, IM_ARRAYSIZE(Buffer));
            if (ImGui::Button("Register Function"))
            {
                lua_Register(Buffer, (int)RunProtectedFunction);
            }
        }
        ImGui::End();
    }




    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}