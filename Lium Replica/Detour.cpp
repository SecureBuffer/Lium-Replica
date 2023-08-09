#include "Detour.h"



Detour::Detour(int target_func, int hook_func) : MemoryOperation((byte*)target_func), hook(hook_func)
{
	DWORD old_protection_;
	VirtualProtect(target, 6, PAGE_EXECUTE_READWRITE, &old_protection_);

	new_bytes.push_back(0x68);
	new_bytes.resize(5);
	*(int*)(new_bytes.data() + 1) = hook;
	new_bytes.push_back(0xc3);

	original_bytes.resize(6);
	memcpy(original_bytes.data(), target, 6);

	Apply();
}

Detour::~Detour()
{
	//Restore();
}

void Detour::Apply()
{
	memcpy(target, new_bytes.data(), 6);
}

void Detour::Restore()
{
	memcpy(target, original_bytes.data(), 6);
}

void WriteBytes(DWORD address, std::vector<BYTE> content, int length)
{
	DWORD oldPFlags;
	VirtualProtect((void*)(address), 1, 0x40, &oldPFlags);
	for (int i = 0; i < length; i++)
	{
		*(unsigned char*)(address + i) = content[i];
	}
}


