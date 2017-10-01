#pragma once

class MemTools
{
public:

	static DWORD64 MemTools::Pattern(const char * pattern, const char * mask, int align = 0, const char * module = nullptr);

private:
	static bool MemTools::Compare(DWORD64 address, const char * pattern, const char * mask);
};