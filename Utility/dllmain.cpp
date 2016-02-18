// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
//#include <iostream>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	// See https://msdn.microsoft.com/en-us/library/windows/desktop/ms682583(v=vs.85).aspx
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		//std::cout << "Utility library: ul_reason_for_call = " << ul_reason_for_call << std::endl;
		break;
	}
	return TRUE;
}

