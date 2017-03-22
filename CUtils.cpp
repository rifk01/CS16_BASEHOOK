#include "Includes.h"

CUtils g_Utils;

DWORD CUtils::SizeOfCodeSection ( std::string sModule )
{
	HMODULE hModule = NULL;

	if ( !( hModule = GetModuleHandle ( sModule.c_str( ) ) ) )
		return 0x0;

	PIMAGE_DOS_HEADER pDsHeader	= PIMAGE_DOS_HEADER ( hModule );
	PIMAGE_NT_HEADERS pPeHeader = PIMAGE_NT_HEADERS ( LONG ( hModule ) + pDsHeader->e_lfanew );
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pPeHeader->OptionalHeader;

	return pOptionalHeader->SizeOfCode;
}

DWORD CUtils::FindPattern ( DWORD dwModuleBase, DWORD dwModuleSize, PBYTE pbPattern, std::string sMask )
{
	size_t uiPos = 0;

	for ( DWORD dwRetOffset = dwModuleBase; dwRetOffset <= dwModuleBase + dwModuleSize; dwRetOffset++ )
	{
		if ( *( PBYTE ) dwRetOffset == pbPattern [ uiPos ] || sMask [ uiPos ] == '?' )
		{
			if ( uiPos + 1 == sMask.length( ) ) return dwRetOffset - ( sMask.length( ) - 1 );
			uiPos++;
		}
		else uiPos = 0;
	}

	return 0x0;
}

VOID CUtils::ConsolePrint ( std::string Text, ... )
{
	CHAR szBuf [ 500 ];
	va_list valist;
	va_start ( valist, Text );
	_vsnprintf_s ( szBuf, sizeof ( szBuf ), Text.c_str( ), valist );
	va_end ( valist );
	strcat_s ( szBuf, "\n" );
	g_pEngine->pfnConsolePrint ( szBuf );
}

BOOL CUtils::WorldToScreen ( vec3_t vWorld, vec3_t &vScreen )
{
	INT iResult = g_pEngine->pTriAPI->WorldToScreen ( vWorld, vScreen );

	if ( vScreen.x < 1.0f && vScreen.y < 1.0f && vScreen.x > - 1.0f && vScreen.y > - 1.0f && !iResult )
	{
        FLOAT flViewport [ 4 ] = { 0 };
        glGetFloatv ( 0x0BA2, flViewport );

		INT ScreenCenterX = flViewport [ 2 ] / 2;
		INT ScreenCenterY = flViewport [ 3 ] / 2;

		vScreen.x = ( vScreen.x * ScreenCenterX ) + ScreenCenterX;
		vScreen.y = ( - vScreen.y * ScreenCenterY ) + ScreenCenterY;
		vScreen.z = 0.0f;

		return TRUE;
	}

	return FALSE;
}

BOOL CUtils::IsEntityValid ( cl_entity_s* pEntity )
{
    if ( pEntity && ( g_pEngine->GetEntityByIndex ( g_pEngine->GetLocalPlayer( )->index )->curstate.iuser2 != pEntity->index ) &&
       ( pEntity != g_pEngine->GetLocalPlayer( ) ) && !( pEntity->curstate.effects & EF_NODRAW ) && pEntity->player &&
        !pEntity->curstate.spectator && pEntity->curstate.solid &&
        !( pEntity->curstate.messagenum < g_pEngine->GetLocalPlayer( )->curstate.messagenum ) )
            return TRUE;
    return FALSE;
}

INT CUtils::StringWidth ( const char* szText, ... )
{
	INT w = 0, h = 0;
	CHAR szBuf [ 500 ] = { 0 };
	va_list valist;

	va_start ( valist,szText );
	_vsnprintf_s ( szBuf, sizeof ( szBuf ), szText, valist );
	va_end ( valist );

	g_pEngine->pfnDrawConsoleStringLen ( szBuf, &w, &h );

	return w;
}

VOID CUtils::DrawString ( BOOL bCenter, INT x, INT y, INT r, INT g, INT b, const char* szText, ... )
{
    va_list valist;
	CHAR szBuf [ 500 ] = { 0 };

	va_start ( valist,szText );
	_vsnprintf_s ( szBuf, sizeof ( szBuf ), szText, valist );
	va_end ( valist );

	g_pEngine->pfnDrawSetTextColor ( r / 255.0f, g / 255.0f, b /255.0f );
	g_pEngine->pfnDrawConsoleString ( x - ( bCenter ? ( g_Utils.StringWidth ( "%s",szBuf ) / 2 ) : 0 ), y, szBuf );
}
