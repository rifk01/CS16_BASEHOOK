#include "Includes.h"

pfnUserMsgHook pTeamInfo = NULL;

INT TeamInfo ( const char* pszName, INT iSize, PVOID pvBuf )
{
    BEGIN_READ ( pvBuf, iSize );

	INT iIndex = READ_BYTE( );
	PCHAR pszTeam = READ_STRING( );

	if ( pszTeam )
    {
        if ( strstr ( pszTeam, "TERROR" ) )
        {
            g_Player[ iIndex ].iTeam = TEAM_TERROR;

            g_Player[ iIndex ].iColor [ ESP_RED ] = ESP_TERROR_RED;
            g_Player[ iIndex ].iColor [ ESP_GREEN ] = ESP_TERROR_GREEN;
            g_Player[ iIndex ].iColor [ ESP_BLUE ] = ESP_TERROR_BLUE;
        }
        else if ( strstr ( pszTeam, "CT" ) )
        {
            g_Player[ iIndex ].iTeam = TEAM_CT;

            g_Player[ iIndex ].iColor [ ESP_RED ] = ESP_CT_RED;
            g_Player[ iIndex ].iColor [ ESP_GREEN ] = ESP_CT_GREEN;
            g_Player[ iIndex ].iColor [ ESP_BLUE ] = ESP_CT_BLUE;
        }
        else
            g_Player[ iIndex ].iTeam = TEAM_UNASSIGNED;
    }

    g_Local.iTeam = g_Player[ g_Local.iIndex ].iTeam;

	return pTeamInfo ( pszName, iSize, pvBuf );
}

typedef struct usermsg_s
{
	int number;
	int size;
	char name[16];
	struct usermsg_s* next;
	pfnUserMsgHook pfn;
} usermsg_t;


VOID HookUserMsgs ( VOID )
{
	DWORD dwUserMsgList = ( DWORD ) g_pEngine->pfnHookUserMsg + 0x1B;

	dwUserMsgList += *( PDWORD ) ( dwUserMsgList ) + 0x4;
	dwUserMsgList += 0xD;

	usermsg_t** ppUserMsgList = ( usermsg_t** )( *( PDWORD ) dwUserMsgList );
	usermsg_t* pUserMsg = *ppUserMsgList;

	while ( pUserMsg )
	{
	    #define HMSG( n ) if( !strcmp ( pUserMsg->name, #n ) ) { p##n = pUserMsg->pfn; pUserMsg->pfn = ##n; }
		HMSG ( TeamInfo )

		pUserMsg = pUserMsg->next;
	}
}
