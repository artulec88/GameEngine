// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
//#include <iostream>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		//std::cout << "ul_reason_for_call = " << ul_reason_for_call;
		break;
	}
	return TRUE;
}
