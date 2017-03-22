#pragma once // HEADER GUARD

#include "Includes.h"

class CUtils
{
    public:
        DWORD SizeOfCodeSection ( std::string sModule );
        DWORD FindPattern ( DWORD dwModuleBase, DWORD dwModuleSize, PBYTE pbPattern, std::string sMask );
        VOID ConsolePrint ( std::string Text, ... );
        BOOL WorldToScreen ( vec3_t vWorld, vec3_t &vScreen );
        BOOL IsEntityValid ( cl_entity_s* pEntity );
        INT StringWidth ( const char* szText, ... );
        VOID DrawString ( BOOL bCenter, INT x, INT y, INT r, INT g, INT b, const char* szText, ... );
} extern g_Utils;
