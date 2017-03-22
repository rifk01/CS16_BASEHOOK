#include "Includes.h"

BOOL WINAPI DllMain ( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )
{
	if ( dwReason == DLL_PROCESS_ATTACH )
	{
	    g_Offsets.DllMain( );
	}

	return TRUE;
}
