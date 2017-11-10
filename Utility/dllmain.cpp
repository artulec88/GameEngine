// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ILogger.h"

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	// See https://msdn.microsoft.com/en-us/library/windows/desktop/ms682583(v=vs.85).aspx
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DEBUG_LOG_UTILITY("The Utility DLL is being ", ((lpReserved == NULL) ? "dynamically" : "statically"), " loaded into the virtual address space of the current process");
		break;
	case DLL_THREAD_ATTACH:
		DEBUG_LOG_UTILITY("The Utility DLL is being attached to the new thread of the current process.");
		break;
	case DLL_THREAD_DETACH:
		DEBUG_LOG_UTILITY("The Utility DLL is being detached from the exiting thread of the current process.");
		break;
	case DLL_PROCESS_DETACH:
		DEBUG_LOG_UTILITY("The Utility DLL is being detached from the current process. ", ((lpReserved == NULL) ? "" : "The process is terminating."));
		break;
	default:
		CRITICAL_LOG_UTILITY("Critical error occured during Utility DLL processing.");
	}
	return TRUE;
}

