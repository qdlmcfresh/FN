// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Psapi.h>
#include <algorithm>
#include <vector>

#include <Windows.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include "FW1FontWrapper.h"
#include <BeaEngine.h>

#pragma comment(lib, "BeaEngine64.lib")
#pragma comment(lib,"FW1FontWrapper.lib")


// TODO: reference additional headers your program requires here
#include <SDK.hpp>
#include "MemTools.h"