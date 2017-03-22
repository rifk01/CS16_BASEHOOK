#include "Includes.h"

enum { TEAM_UNASSIGNED, TEAM_TERROR, TEAM_CT };
enum { ESP_RED, ESP_GREEN, ESP_BLUE };

class CLocalPlayer
{
    public:
        INT iTeam,
            iIndex,
            iScreenCenterX,
            iScreenCenterY;

        VOID Update ( VOID );
} extern g_Local;

struct sound_s
{
    BOOL bIsValid,
         bIsOnScreen;

    vec3_t vWorldOrigin,
           vScreenOrigin;

    DWORD dwStartTime;
};

class CPlayer
{
    public:
        BOOL bIsValid,
             bIsOnScreen;

        INT iTeam,
            iColor [ 3 ];

        vec3_t vWorldOrigin,
               vScreenOrigin;

        CHAR szIngameName [ MAX_SCOREBOARDNAME ];

        sound_s sound;

} extern g_Player [ 33 ];
