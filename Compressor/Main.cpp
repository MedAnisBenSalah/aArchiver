#include "Main.h"

BOOL APIENTRY DLLMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch(dwReason)
	{
		// On load
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule);
			break;

		// On unload
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) CArchiver *CreateArchiver()
{
	return (new CArchiver()); 
}

extern "C" __declspec(dllexport) void DestroyArchiver(CArchiver *pInstance)
{
	SAFE_DELETE(pInstance); 
}