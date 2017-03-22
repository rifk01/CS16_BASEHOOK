#include "Includes.h"

CLocalPlayer g_Local;
CPlayer g_Player [ 33 ];

VOID CLocalPlayer::Update ( VOID )
{
    g_Local.iIndex = g_pEngine->GetLocalPlayer( )->index;

    FLOAT flViewport [ 4 ] = { 0 };
    glGetFloatv ( 0x0BA2, flViewport );

    g_Local.iScreenCenterX = flViewport [ 2 ] / 2;
    g_Local.iScreenCenterY = flViewport [ 3 ] / 2;

    for ( UINT uiIndex = 1; uiIndex <= 32; uiIndex++ )
    {
        cl_entity_s* pEntity = g_pEngine->GetEntityByIndex ( uiIndex );

        hud_player_info_t pPlayerInfo;
        g_pEngine->pfnGetPlayerInfo ( uiIndex, &pPlayerInfo );

        if ( !IsBadStringPtr ( pPlayerInfo.name, MAX_SCOREBOARDNAME ) )
            memcpy ( g_Player[ uiIndex ].szIngameName, pPlayerInfo.name, MAX_SCOREBOARDNAME );

        g_Utils.IsEntityValid ( pEntity ) ?
            g_Player[ uiIndex ].bIsValid = TRUE :
                g_Player[ uiIndex ].bIsValid = FALSE;

        if ( g_Player[ uiIndex ].bIsValid )
        {
            g_Player[ uiIndex ].vWorldOrigin = pEntity->origin;

            g_Utils.WorldToScreen ( g_Player[ uiIndex ].vWorldOrigin, g_Player[ uiIndex ].vScreenOrigin ) ?
                g_Player[ uiIndex ].bIsOnScreen = TRUE :
                    g_Player[ uiIndex ].bIsOnScreen = FALSE;
        }

    }

}
