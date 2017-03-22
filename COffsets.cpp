#include "Includes.h"

COffsets g_Offsets;

cl_clientfunc_t* g_pClient = NULL;
cl_clientfunc_t g_Client;

cl_enginefunc_t* g_pEngine = NULL;
cl_enginefunc_t g_Engine;

engine_studio_api_t* g_pStudio = NULL;
engine_studio_api_t g_Studio;

DWORD WINAPI FindOffsets ( LPVOID lpParam )
{
	HMODULE hmCLIENT = NULL;
	HMODULE hmHW = NULL;

	for ( ; !hmCLIENT; Sleep ( 10 ) )
		hmCLIENT = GetModuleHandle ( "client.dll" );

	for ( ; !hmHW; Sleep ( 10 ) )
		hmHW = GetModuleHandle ( "hw.dll" );

	g_Offsets.ClientDll.dwModuleBase = ( DWORD ) hmCLIENT;
	g_Offsets.ClientDll.dwCodeSectionSize = g_Utils.SizeOfCodeSection ( "client.dll" );

	if ( !g_Offsets.ClientDll.dwCodeSectionSize )
	{
		MessageBox ( 0, "Could not obtain the modulesize", "Error", MB_ICONINFORMATION );
		return 0x0;
	}

	g_Offsets.HWDll.dwModuleBase = ( DWORD ) hmHW;
	g_Offsets.HWDll.dwCodeSectionSize = g_Utils.SizeOfCodeSection ( "hw.dll" );

	if ( !g_Offsets.HWDll.dwCodeSectionSize )
	{
		MessageBox ( 0, "Could not obtain the modulesize", "Error", MB_ICONINFORMATION );
		return 0x0;
	}

/*	ENGFUNCS
	07A206E8   50               PUSH EAX
	07A206E9   FF15 A01BB007    CALL DWORD PTR DS:[7B01BA0]                           ; hw.05CEE580
	07A206EF   8B9C24 B0000000  MOV EBX,DWORD PTR SS:[ESP+B0]
*/
	DWORD dwEngFuncs = g_Utils.FindPattern ( g_Offsets.ClientDll.dwModuleBase, g_Offsets.ClientDll.dwCodeSectionSize,
		( PBYTE ) "\x50"
		          "\xFF\x15\x00\x00\x00\x00"
				  "\x8B\x9C\x24\x00\x00\x00\x00",
		( PCHAR ) "x"
		          "xx????"
				  "xxx????" );
	g_Offsets.ClientDll.dwEngFuncs = *( PDWORD ) ( dwEngFuncs + 0x3 );

/*  ENGSTUDIO
	07A2ADF0 > 837C24 04 01                 CMP DWORD PTR SS:[ESP+4],1
	07A2ADF5   74 03                        JE SHORT client.07A2ADFA
	07A2ADF7   33C0                         XOR EAX,EAX
	07A2ADF9   C3                           RETN
*/
	DWORD dwEngStudio = g_Utils.FindPattern ( g_Offsets.ClientDll.dwModuleBase, g_Offsets.ClientDll.dwCodeSectionSize,
		( PBYTE ) "\x83\x7C\x24\x04\x01"
		          "\x74\x03"
				  "\x33\xC0"
				  "\xC3",
		( PCHAR ) "xxxxx"
		          "xx"
				  "xx"
				  "x" );
	g_Offsets.ClientDll.dwEngStudio = *( PDWORD ) ( dwEngStudio + 0x1A );

/*  PRESDYNAMICSOUND
	05D72BC0   83EC 48          SUB ESP,48
	05D72BC3   A1 5CF74006      MOV EAX,DWORD PTR DS:[640F75C]
	05D72BC8   53               PUSH EBX
	05D72BC9   55               PUSH EBP
	05D72BCA   56               PUSH ESI
*/
	g_Offsets.HWDll.dwPreSDynamicSound = g_Utils.FindPattern ( g_Offsets.HWDll.dwModuleBase, g_Offsets.HWDll.dwCodeSectionSize,
		( PBYTE ) "\x83\xEC\x48"
		          "\xA1\x00\x00\x00\x00"
				  "\x53"
				  "\x55"
				  "\x56",
		( PCHAR ) "xxx"
		          "x????"
				  "x"
				  "x"
				  "x" );

/*  EXPORTTABLE
	05D58390   8B4424 04        MOV EAX,DWORD PTR SS:[ESP+4]
	05D58394   6A 00            PUSH 0
	05D58396   68 20E3F306      PUSH hw.06F3E320
*/
	g_Offsets.HWDll.dwExportTable = g_Utils.FindPattern ( g_Offsets.HWDll.dwModuleBase, g_Offsets.HWDll.dwCodeSectionSize,
		( PBYTE ) "\x8B\x44\x24\x04"
		          "\x6A\x00"
				  "\x68\x00\x00\x00\x00",
		( PCHAR ) "xxxx"
		          "xx"
				  "x????" );

	if ( !g_Offsets.ClientDll.dwEngFuncs || !g_Offsets.ClientDll.dwEngStudio ||
		 !g_Offsets.HWDll.dwPreSDynamicSound || !g_Offsets.HWDll.dwExportTable )
	{
		MessageBox ( 0, "Could not obtain all offsets", "Error", MB_ICONINFORMATION );
		return 0x0;
	}

    g_pClient = ( cl_clientfunc_t* ) *( ( PDWORD ) ( g_Offsets.HWDll.dwExportTable + 0x7 ) );
	g_pEngine = ( cl_enginefunc_t* ) ( g_Offsets.ClientDll.dwEngFuncs );
	g_pStudio = ( engine_studio_api_t* ) ( g_Offsets.ClientDll.dwEngStudio );

    memcpy ( &g_Client, g_pClient, sizeof ( cl_clientfunc_t ) );
    memcpy ( &g_Engine, g_pEngine, sizeof ( cl_enginefunc_t ) );
    memcpy ( &g_Studio, g_pStudio, sizeof ( engine_studio_api_t ) );

    InterceptFunctions( );
    HookUserMsgs( );

	return 0x0;
}

VOID COffsets::DllMain ( VOID )
{
    CreateThread ( NULL, 0, FindOffsets, NULL, 0, NULL );
}
