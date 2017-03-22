#pragma once // HEADER GUARD
#pragma comment ( lib, "opengl32.lib" )

// WIN PSDK INCLUDES
#include <Windows.h>
#include <String>
#include <GL/GL.h>

// HLSDK INCLUDES
#include "ValveSDK/engine/wrect.h"
#include "ValveSDK/engine/cl_dll.h"
#include "ValveSDK/engine/r_studioint.h"
#include "ValveSDK/engine/cl_entity.h"
#include "ValveSDK/misc/com_model.h"
#include "ValveSDK/engine/triangleapi.h"
#include "ValveSDK/engine/pmtrace.h"
#include "ValveSDK/engine/pm_defs.h"
#include "ValveSDK/engine/pm_info.h"
#include "ValveSDK/common/ref_params.h"
#include "ValveSDK/common/studio_event.h"
#include "ValveSDK/common/net_api.h"
#include "ValveSDK/common/r_efx.h"
#include "ValveSDK/engine/cvardef.h"
#include "ValveSDK/engine/util_vector.h"
#include "ValveSDK/misc/parsemsg.h"
#include "ValveSDK/engine/studio.h"
#include "ValveSDK/engine/event_args.h"
#include "ValveSDK/engine/event_flags.h"
#include "ValveSDK/common/event_api.h"
#include "ValveSDK/common/screenfade.h"
#include "ValveSDK/engine/keydefs.h"
#include "ValveSDK/engine/util_vector.h"

// PROJECT INCLUDES
#include "COffsets.h"
#include "CUtils.h"
#include "EngineHook.h"
#include "CPlayer.h"
#include "UserMsgHook.h"
#include "ProjectDefines.h"

// ENGINE PTR
extern cl_clientfunc_t* g_pClient;
extern cl_clientfunc_t g_Client;

extern cl_enginefunc_t* g_pEngine;
extern cl_enginefunc_t g_Engine;

extern engine_studio_api_t* g_pStudio;
extern engine_studio_api_t g_Studio;
