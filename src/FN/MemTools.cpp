#include "stdafx.h"

DWORD64 MemTools::Pattern(const char * pattern, const char* mask, int align, const char * module)
{
	HMODULE hModule = GetModuleHandleA(module);
	MODULEINFO moduleInfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(MODULEINFO));

	DWORD64 moduleBase = reinterpret_cast<DWORD64>(moduleInfo.lpBaseOfDll);
	DWORD64 moduleSize = (DWORD64)moduleInfo.SizeOfImage;
	DWORD64 patternSize = (DWORD64)strlen(mask);

	for (int i = 0; i < moduleSize - patternSize; i++)
	{
		if (Compare(moduleBase + i, pattern, mask))
			return (moduleBase + i + align);
	}

	return 0;
}

bool MemTools::Compare(DWORD64 address, const char * pattern, const char * mask)
{
	for (int i = 0; i < strlen(mask); i++)
	{
		if (mask[i] == 'x' && *(char*)(address + i) != pattern[i])
			return false;
	}

	return true;
}