#include "stdafx.h"

using namespace SDK;


void init(HMODULE hModule)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	DWORD64 baseAddr = reinterpret_cast<DWORD64>(GetModuleHandleA(nullptr));
	DWORD64 instructionOffset = MemTools::Pattern("\x48\x8B\x15\x00\x00\x00\x00\x45\x33\xC0\xE8\x00\x00\x00\x00", "xxx????xxxx????", 3);
	DWORD64 UWorldOffset = *reinterpret_cast<DWORD*>(instructionOffset) + instructionOffset + sizeof(DWORD);

	while (!GetAsyncKeyState(VK_DELETE) & 1)
	{
		system("cls");

		UWorld *pUWorld = *reinterpret_cast<UWorld**>(UWorldOffset);
		if (pUWorld)
			printf("World is valid @ 0x%I64X\n", pUWorld);
		else
		{
			printf("World is invalid!\n");
			continue;
		}

		if (pUWorld->OwningGameInstance)
			printf("World->OwningGameInstance is valid @ 0x%I64X\n", pUWorld->OwningGameInstance);
		else
		{
			printf("World->OwningGameInstance is invalid!\n");
			continue;
		}

		if (!pUWorld->OwningGameInstance->LocalPlayers.IsValidIndex(0))
			continue;

		ULocalPlayer *pLocalPlayer = pUWorld->OwningGameInstance->LocalPlayers[0];
		if (!pLocalPlayer)
			continue;

		if (!pLocalPlayer->ViewportClient)
			continue;

		if (!pLocalPlayer->ViewportClient->World)
			continue;

		UWorld *pRealWorld = pLocalPlayer->ViewportClient->World;
		if (!pRealWorld)
			continue;

		if (!pRealWorld->OwningGameInstance)
			continue;

		if (!pRealWorld->OwningGameInstance->LocalPlayers[0])
			continue;

		if (!pRealWorld->OwningGameInstance->LocalPlayers[0]->PlayerController)
			continue;

		AFortPawn *pLocalPlayerPawn = static_cast<AFortPawn*>(pRealWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->Pawn);
		if (!pLocalPlayerPawn)
			continue;

		if (!pRealWorld->PersistentLevel)
			continue;

		for (int i = 0; i < pRealWorld->PersistentLevel->AActors.Num(); i++)
		{
			if (!pRealWorld->PersistentLevel->AActors.IsValidIndex(i))
				continue;

			AFortPawn * pPlayerPawn = static_cast<AFortPawn*>(pRealWorld->PersistentLevel->AActors[i]);
			if (!pPlayerPawn)
				continue;

			if (pPlayerPawn->GetName().find("PlayerPawn") == std::string::npos)
				continue;

			AFortPlayerStateAthena * pPlayerState = static_cast<AFortPlayerStateAthena*>(pPlayerPawn->PlayerState);
			if (!pPlayerState)
				continue;

			if (pPlayerState->PlayerName.IsValid())
			{
				wprintf(L"Player: %s | Health: %.0f | Shield: %.0f | Distance: %.2f meters.\n", pPlayerState->PlayerName.c_str(), pPlayerState->CurrentHealth, pPlayerState->CurrentShield, pPlayerPawn->GetDistanceTo(pLocalPlayerPawn) / 100.f);
			}
		}

		Sleep(1000);
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