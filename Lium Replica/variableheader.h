#pragma once
#include "Graphics.h"
#include <map>
#include <string>

std::map<std::string, MemoryOperation*> DetourHooks{};
bool shouldRemoveEndSceneInjection = false;
int __stdcall EndScene(LPDIRECT3DDEVICE9 device);
