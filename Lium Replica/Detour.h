#pragma once
#include <vector>
#include <memoryapi.h>

class MemoryOperation
{
	typedef unsigned char  byte;
public:
	std::vector<byte> original_bytes{};
	std::vector<byte> new_bytes{};
	byte* target{};

	MemoryOperation(byte* target_arg) :target{ target_arg } {}
	MemoryOperation(byte* target_arg, std::initializer_list<byte> bytes) : target{ target_arg }, new_bytes{ bytes } {}

	virtual ~MemoryOperation() {}
	virtual void Apply() = 0;
	virtual void Restore() = 0;
};


class Detour : public MemoryOperation
{
	typedef unsigned char byte;

public:
	Detour(int target_func, int hook_func);
	~Detour();

	void Apply();
	void Restore();

private:
	int hook;
	std::vector<byte> new_bytes;
	std::vector<byte> original_bytes;
};