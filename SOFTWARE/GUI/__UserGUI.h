#pragma once

#include "GUIDefine.h"


/*********************************************************************
*
*       Declartion
*
**********************************************************************
*/

BUTTON_Handle __User_BUTTON_CreateEx (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id);
void __User_BUTTON_Callback (WM_MESSAGE* pMsg);
WM_HWIN __User_GUI_CreateDialogBox (const GUI_WIDGET_CREATE_INFO* paWidget, int NumWidgets, WM_CALLBACK* cb, WM_HWIN hParent, int x0, int y0, ResourceMap_struct* RMs);
BUTTON_Handle __User_BUTTON_CreateIndirect (const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);

