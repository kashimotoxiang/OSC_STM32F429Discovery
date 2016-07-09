#pragma once

#include "GUIDefine.h"

/* ResourceMap---------------------------------------------------------*/

/*********************************************************************
*
*       Declartion
*
**********************************************************************
*/

WM_HWIN APPEND_CreateWindow (void);
void RMSwitch (u8 state, ResourceMap_struct RMs);
WM_HWIN Text_CreateWindow (void);

extern ResourceMap_struct NumPad_RMs;

