#include "stdafx.h"

using namespace SDK;

UWorld *pUWorld;

void init(HMODULE hModule)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	DWORD64 baseAddr = reinterpret_cast<DWORD64>(GetModuleHandleA(nullptr));
	DWORD64 instructionOffset = MemTools::Pattern("\x48\x8B\x15\x00\x00\x00\x00\x45\x33\xC0\xE8\x00\x00\x00\x00", "xxx????xxxx????", 3);
	DWORD64 UWorldOffset = *reinterpret_cast<DWORD*>(instructionOffset) + instructionOffset + sizeof(DWORD);
	
	pUWorld = *reinterpret_cast<UWorld**>(UWorldOffset);
	

	while (!GetAsyncKeyState(VK_DELETE) & 1)
	{
		if (pUWorld && pUWorld->OwningGameInstance)
		{
			ULocalPlayer *localPlayer = pUWorld->OwningGameInstance->LocalPlayers[0];

			if (localPlayer)
			{
				AFortPawn *localPlayerPawn = static_cast<AFortPawn*>(localPlayer->PlayerController->Pawn);
				if (localPlayerPawn)
				{
					AFortPlayerState *localPlayerState = static_cast<AFortPlayerState*>(localPlayerPawn->PlayerState);
					if (localPlayerState)
					{
						wprintf(L"My Name: %s", localPlayerState->PlayerName.c_str());
						break;
					}
				}
			}
		}

		Sleep(10);
	}

	FreeConsole();
		FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(init), hModule, 0, 0);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}