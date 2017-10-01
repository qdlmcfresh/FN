#include "stdafx.h"

using namespace SDK;

UWorld *pUWorld;

void init(HMODULE hModule)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	const auto addressObj = MemTools::Pattern("\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xD6", "xxx????x????x????x????xxx");
	const auto offsetObj = *reinterpret_cast<uint32_t*>(addressObj + 3);
	UObject::GObjects = reinterpret_cast<FUObjectArray*>(addressObj + 7 + offsetObj);

	const auto addressNames = MemTools::Pattern("\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x75\x50\xB9\x00\x00\x00\x00\x48\x89\x5C\x24", "xxx????xxxxxx????xxxx");
	const auto offsetNames = *reinterpret_cast<uint32_t*>(addressNames + 3);
	FName::GNames = *reinterpret_cast<TNameEntryArray**>(addressNames + 7 + offsetNames);

	DWORD64 baseAddr = reinterpret_cast<DWORD64>(GetModuleHandleA(nullptr));
	DWORD64 instructionOffset = MemTools::Pattern("\x48\x8B\x15\x00\x00\x00\x00\x45\x33\xC0\xE8\x00\x00\x00\x00", "xxx????xxxx????", 3);
	DWORD64 UWorldOffset = *reinterpret_cast<DWORD*>(instructionOffset) + instructionOffset + sizeof(DWORD);

	pUWorld = *reinterpret_cast<UWorld**>(UWorldOffset);

	while (!GetAsyncKeyState(VK_DELETE) & 1)
	{
		system("cls");
		if (!pUWorld || !pUWorld->OwningGameInstance || !pUWorld->PersistentLevel)
			continue;

		ULocalPlayer *localPlayer = pUWorld->OwningGameInstance->LocalPlayers[0];
		if (!localPlayer || !localPlayer->PlayerController)
			continue;

		AFortPawn *localPlayerPawn = static_cast<AFortPawn*>(localPlayer->PlayerController->Pawn);
		if (!localPlayerPawn)
			continue;

		AFortPlayerStateAthena *localPlayerState = static_cast<AFortPlayerStateAthena*>(localPlayerPawn->PlayerState);
		if (!localPlayerState)
			continue;

		for (int i = 0; i < pUWorld->PersistentLevel->AActors.Num(); i++)
		{
			AFortPawn *pPlayerPawn = static_cast<AFortPawn*>(pUWorld->PersistentLevel->AActors[i]);
			if (!pPlayerPawn || pPlayerPawn == localPlayerPawn)
				continue;

			if (pPlayerPawn->GetName().find("PlayerPawn_Athena_C") == std::string::npos)
				continue;

			AFortPlayerStateAthena *pPlayerState = static_cast<AFortPlayerStateAthena*>(pPlayerPawn->PlayerState);
			if (!pPlayerState)
				continue;

			if (!pPlayerState->PlayerName.IsValid())
				continue;

			wprintf(L"Player: %d has name: %s\n", i, pPlayerState->PlayerName.c_str());
		}

		Sleep(10);
	}

	printf("Ejecting...\n");
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