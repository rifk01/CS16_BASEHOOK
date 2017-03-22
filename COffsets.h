#pragma once // HEADER GUARD

#include "Includes.h"

typedef struct ClientDll_s
{
	DWORD dwModuleBase;
	DWORD dwCodeSectionSize;

	DWORD dwEngFuncs;
	DWORD dwEngStudio;

} ClientDll_t;

typedef struct HWDll_s
{
	DWORD dwModuleBase;
	DWORD dwCodeSectionSize;

	DWORD dwPreSDynamicSound;
	DWORD dwExportTable;
} HWDll_t;

class COffsets
{
    public:
        ClientDll_t ClientDll;
        HWDll_t HWDll;
        VOID DllMain ( VOID );
    private:
} extern g_Offsets;
