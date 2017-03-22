#include "Includes.h"

VOID HUD_Frame ( DOUBLE dblTime )
{
	static BOOL bDidOnce = FALSE;

	if ( !bDidOnce )
	{
		if ( !g_pEngine->Con_IsVisible( ) )
            g_pEngine->pfnClientCmd ( "toggleconsole" );

		g_Utils.ConsolePrint ( CONSOLE_HACKINFO );
		g_Utils.ConsolePrint ( "Binary build date: %s @ %s\n", __DATE__, __TIME__ );

		bDidOnce = TRUE;
	}

	g_Client.HUD_Frame ( dblTime );
}

VOID HUD_Redraw ( FLOAT flTime, INT iIntermission )
{
    g_Client.HUD_Redraw ( flTime, iIntermission );

    g_Local.Update( );

    g_Utils.DrawString ( FALSE, 140, 10, 255, 255, 255, "Screen Resolution: %ix%i",
                        g_Local.iScreenCenterX * 2, g_Local.iScreenCenterY * 2 );

    for ( UINT uiIndex = 1; uiIndex <= 32; uiIndex++ )
    {
        vec3_t vTempOrigin;

        g_Player[ uiIndex ].sound.bIsValid = g_Player[ uiIndex ].sound.dwStartTime + 1250 > GetTickCount( );

        BOOL bFarEsp = !g_Player[ uiIndex ].bIsValid && g_Player[ uiIndex ].sound.bIsValid &&
            g_Utils.WorldToScreen ( g_Player[ uiIndex ].sound.vWorldOrigin, vTempOrigin );

        BOOL bValidEsp = g_Player[ uiIndex ].bIsValid && g_Player[ uiIndex ].bIsOnScreen;

        if ( bValidEsp )
            vTempOrigin = g_Player [ uiIndex ].vScreenOrigin;

        if ( !bValidEsp && !bFarEsp )
            continue;

        g_Utils.DrawString ( TRUE, vTempOrigin.x, vTempOrigin.y,
        g_Player[ uiIndex ].iColor [ ESP_RED ], g_Player[ uiIndex ].iColor [ ESP_GREEN ], g_Player[ uiIndex ].iColor [ ESP_BLUE ],
        g_Player[ uiIndex ].szIngameName );
    }

}

typedef VOID ( *DynamicSound_t ) ( int, int, char*, float*, float, float, int, int );
DynamicSound_t pDynamicSound;

VOID DynamicSound ( int index, int entchannel, char* sample, float* origin, float volume, float attenuation, int timeofs, int pitch )
{
    pDynamicSound ( index, entchannel, sample, origin, volume, attenuation, timeofs, pitch );

	if ( index >= 1 && index <= 32 && origin && index != g_Local.iIndex )
	{
	    g_Player[ index ].sound.vWorldOrigin = origin;
	    g_Player[ index ].sound.dwStartTime = GetTickCount( );
	}

}

void *Detour(BYTE *pbFunc,BYTE *pbNewFunc,int iLen)
{
	BYTE *pbGate=new BYTE[iLen+5];
	DWORD dwOld;
	VirtualProtect(pbFunc,iLen,PAGE_EXECUTE_READWRITE,&dwOld);
	memcpy(pbGate,pbFunc,iLen);
	pbGate+=iLen;
	pbGate[0]=0xE9;
	*(DWORD*)(pbGate+1)=DWORD(pbFunc+iLen-pbGate)-5;
	pbFunc[0]=0xE9;
	*(DWORD*)(pbFunc+1)=DWORD(pbNewFunc-pbFunc)-5;
	for(int i=5;i<iLen;i++)
		pbFunc[i]=0xCC;
	VirtualProtect(pbFunc,iLen,dwOld,&dwOld);
	return (pbGate-iLen);
}

VOID InterceptFunctions ( VOID )
{
    #define InterceptFunction( FuncName ) InterlockedExchangePointer ( &g_pClient->##FuncName, ##FuncName );

    InterceptFunction ( HUD_Redraw );
    InterceptFunction ( HUD_Frame );

    pDynamicSound = ( DynamicSound_t ) Detour ( ( PBYTE ) g_Offsets.HWDll.dwPreSDynamicSound, ( PBYTE ) DynamicSound, 8 );
}
