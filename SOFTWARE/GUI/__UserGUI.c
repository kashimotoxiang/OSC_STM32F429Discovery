#include "__UserGUI.h"

#include "BUTTON_Private.h"
#include "BUTTON.h"
#define BUTTON_H2P(h) (BUTTON_Obj*) GUI_ALLOC_h2p(h)

/*********************************************************************
*
*       extern data
*
**********************************************************************
*/
/*********************************************************************
*
*       global data
*
**********************************************************************
*/
/*********************************************************************
*
*       Static routines
*
**********************************************************************
*/
/*********************************************************************
*
*       _Paint
*/
static void __User_GUI_DRAW__Draw (GUI_DRAW_HANDLE hDrawObj, int x, int y) {
	if (hDrawObj) {
		GUI_DRAW* pDrawObj;
		pDrawObj = (GUI_DRAW *)GUI_ALLOC_h2p(hDrawObj);
		pDrawObj->pConsts->pfDraw(hDrawObj, WM_HBKWIN, x, y);
	}
}

static void _Paint (BUTTON_Obj* pObj, BUTTON_Handle hObj) {
	const char* s = NULL;
	unsigned int Index;
	int State, PressedState, ColorIndex;
	GUI_RECT rClient, rInside;
	State = pObj->Widget.State;
	PressedState = (State & BUTTON_STATE_PRESSED) ? 1 : 0;
	ColorIndex = (WM__IsEnabled(hObj)) ? PressedState : 2;
	GUI_SetFont(pObj->Props.pFont);
	GUI_DEBUG_LOG ("BUTTON: Paint(..)\n");
	if (pObj->hpText) {
		s = (const char*)GUI_ALLOC_h2p(pObj->hpText);
	}
	GUI_GetClientRect(&rClient);
	/* Start drawing */
	rInside = rClient;
	/* Draw the 3D effect (if configured) */

	int EffectSize;
	if ((PressedState) == 0) {
		pObj->Widget.pEffect->pfDrawUp(); /* _WIDGET_EFFECT_3D_DrawUp(); */
		EffectSize = pObj->Widget.pEffect->EffectSize;
	}
	else {
		LCD_SetColor(0x000000);
		GUI_DrawRect(rClient.y0, rClient.x0, rClient.x1, rClient.y1);
		EffectSize = 1;
	}
	GUI__ReduceRect(&rInside, &rInside, EffectSize);

	/* Draw background */
	LCD_SetBkColor(pObj->Props.aBkColor[ColorIndex]);
	LCD_SetColor(pObj->Props.aTextColor[ColorIndex]);
	WM_SetUserClipRect(&rInside);
	GUI_Clear();
	/* Draw bitmap.
	If we have only one, we will use it.
	If we have to we will use the second one (Index 1) for the pressed state
	*/
	if (ColorIndex < 2) {
		Index = (pObj->ahDrawObj[BUTTON_BI_PRESSED] && PressedState) ? BUTTON_BI_PRESSED : BUTTON_BI_UNPRESSED;
	}
	else {
		Index = pObj->ahDrawObj[BUTTON_BI_DISABLED] ? BUTTON_BI_DISABLED : BUTTON_BI_UNPRESSED;
	}
	//GUI_DRAW__Draw(pObj->ahDrawObj[Index], WM_HBKWIN,0, 0);
	/* Draw the actual button (background and text) */ {
		GUI_RECT r;
		r = rInside;

		if (PressedState) {
			GUI_MoveRect(&r, BUTTON_3D_MOVE_X, BUTTON_3D_MOVE_Y);
		}

		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_DispStringInRect(s, &r, pObj->Props.Align);
	}
	/* Draw focus */
	if (State & BUTTON_STATE_FOCUS) {
		LCD_SetColor(GUI_BLACK);
		GUI_DrawFocusRect(&rClient, 2);
	}
	WM_SetUserClipRect(NULL);
}

/*********************************************************************
*
*       _Delete
*
* Delete attached objects (if any)
*/
static void _Delete (BUTTON_Obj* pObj) {
	GUI_ALLOC_FreePtr(&pObj->hpText);
	GUI_ALLOC_FreePtr(&pObj->ahDrawObj[0]);
	GUI_ALLOC_FreePtr(&pObj->ahDrawObj[1]);
}

/*********************************************************************
*
*       _ButtonPressed
*/
static void _ButtonPressed (BUTTON_Handle hObj, BUTTON_Obj* pObj) {
	WIDGET_OrState(hObj, BUTTON_STATE_PRESSED);
	if (pObj->Widget.Win.Status & WM_SF_ISVIS) {
		WM_NotifyParent(hObj, WM_NOTIFICATION_CLICKED);
	}
}

/*********************************************************************
*
*       _ButtonReleased
*/
static void _ButtonReleased (BUTTON_Handle hObj, BUTTON_Obj* pObj, int Notification) {
	WIDGET_AndState(hObj, BUTTON_STATE_PRESSED);
	if (pObj->Widget.Win.Status & WM_SF_ISVIS) {
		WM_NotifyParent(hObj, Notification);
	}
	if (Notification == WM_NOTIFICATION_RELEASED) {
		GUI_DEBUG_LOG ("BUTTON: Hit\n");
		GUI_StoreKey(pObj->Widget.Id);
	}
}

/*********************************************************************
*
*       _OnTouch
*/
static void _OnTouch (BUTTON_Handle hObj, BUTTON_Obj* pObj, WM_MESSAGE* pMsg) {
	const GUI_PID_STATE* pState = (const GUI_PID_STATE*)pMsg->Data.p;
#if BUTTON_REACT_ON_LEVEL
	if (!pMsg->Data.p) {  /* Mouse moved out */
		_ButtonReleased (hObj, pObj, WM_NOTIFICATION_MOVED_OUT);
	}
#else
	if (pMsg->Data.p) { /* Something happened in our area (pressed or released) */
		if (pState->Pressed) {
			if ((pObj->Widget.State & BUTTON_STATE_PRESSED) == 0) {
				_ButtonPressed(hObj, pObj);
			}
		}
		else {
			/* React only if button was pressed before ... avoid problems with moving / hiding windows above (such as dropdown) */
			if (pObj->Widget.State & BUTTON_STATE_PRESSED) {
				_ButtonReleased(hObj, pObj, WM_NOTIFICATION_RELEASED);
			}
		}
	}
	else {
		_ButtonReleased(hObj, pObj, WM_NOTIFICATION_MOVED_OUT);
	}

#endif
}

/*********************************************************************
*
*       _OnPidStateChange
*/
#if BUTTON_REACT_ON_LEVEL
static void _OnPidStateChange (BUTTON_Handle hObj, BUTTON_Obj * pObj, WM_MESSAGE * pMsg) {
	const WM_PID_STATE_CHANGED_INFO * pState = (const WM_PID_STATE_CHANGED_INFO *)pMsg->Data.p;
	if ((pState->StatePrev == 0) && (pState->State == 1)) {
		if ((pObj->Widget.State & BUTTON_STATE_PRESSED) == 0) {
			_ButtonPressed (hObj, pObj);
		}
	}
	else if ((pState->StatePrev == 1) && (pState->State == 0)) {
		if (pObj->Widget.State & BUTTON_STATE_PRESSED) {
			_ButtonReleased (hObj, pObj, WM_NOTIFICATION_RELEASED);
		}
	}
}
#endif


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       __User_BUTTON_CreateIndirect
*/
BUTTON_Handle __User_BUTTON_CreateIndirect (const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb) {
	BUTTON_Handle hThis;
	GUI_USE_PARA (cb);
	hThis = __User_BUTTON_CreateEx(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0,
	                               pCreateInfo->xSize, pCreateInfo->ySize,
	                               hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id);
	BUTTON_SetText(hThis, pCreateInfo->pName);
	return hThis;
}

/*********************************************************************
*
*       __User_BUTTON_CreateEx
*/
BUTTON_Handle __User_BUTTON_CreateEx (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id) {
	BUTTON_Handle hObj;
	GUI_USE_PARA (ExFlags);
	/* Create the window */
	WM_LOCK ();
	hObj = WM_CreateWindowAsChild(x0, y0, xsize, ysize, hParent, WinFlags, __User_BUTTON_Callback, sizeof (BUTTON_Obj) - sizeof (WM_Obj));
	if (hObj) {
		BUTTON_Obj* pObj = (BUTTON_Obj*)BUTTON_H2P(hObj);
		/* init widget specific variables */
		WIDGET__Init(&pObj->Widget, Id, WIDGET_STATE_FOCUSSABLE);
		/* init member variables */
		BUTTON_INIT_ID (pObj);
		pObj->Props = BUTTON__DefaultProps;
	}
	else {
		GUI_DEBUG_ERROROUT_IF (hObj == 0, "BUTTON_Create failed")
	}
	WM_UNLOCK ();
	return hObj;
}

/*********************************************************************
*
*       __User_BUTTON_Callback
*/
void __User_BUTTON_Callback (WM_MESSAGE* pMsg) {
	BUTTON_Handle hObj = pMsg->hWin;
	BUTTON_Obj* pObj = (BUTTON_Obj*)BUTTON_H2P(hObj);
	/* Let widget handle the standard messages */
	if (WIDGET_HandleActive(hObj, pMsg) == 0) {
		return;
	}
	switch (pMsg->MsgId) {
#if BUTTON_REACT_ON_LEVEL
		case WM_PID_STATE_CHANGED:
			_OnPidStateChange (hObj, pObj, pMsg);
			return;      /* Message handled. Do not call WM_DefaultProc, because the window may have been destroyed */
#endif
		case WM_TOUCH:
			_OnTouch(hObj, pObj, pMsg);
			return; /* Message handled. Do not call WM_DefaultProc, because the window may have been destroyed */
		case WM_PAINT:
			GUI_DEBUG_LOG ("BUTTON: _BUTTON_Callback(WM_PAINT)\n");
			_Paint(pObj, hObj);
			return;
		case WM_DELETE:
			GUI_DEBUG_LOG ("BUTTON: _BUTTON_Callback(WM_DELETE)\n");
			_Delete(pObj);
			break; /* No return here ... WM_DefaultProc needs to be called */
#if 0 /* TBD: Button should react to space & Enter */
		case WM_KEY:
		{
			int PressedCnt = ((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt;
			int Key = ((WM_KEY_INFO*)(pMsg->Data.p))->Key;
			if (PressedCnt > 0) {   /* Key pressed? */
				switch (Key) {
					case ' ':
						_ButtonPressed (hObj, pObj);
						return;
				}
			}
			else {
				switch (Key) {
					case ' ':
						_ButtonReleased (hObj, pObj, WM_NOTIFICATION_RELEASED);
						return;
				}
			}
		}
		break;
#endif
	}
	WM_DefaultProc(pMsg);
}

/*********************************************************************
*
*       GUI_CreateDialogbox
*/
WM_HWIN __User_GUI_CreateDialogBox (const GUI_WIDGET_CREATE_INFO* paWidget, int NumWidgets, WM_CALLBACK* cb, WM_HWIN hParent, int x0, int y0, ResourceMap_struct* RMs) {
	WM_HWIN hChild;
	int i = 0;
	(*RMs).Num = NumWidgets;
	while (--NumWidgets > 0) {
		hChild = paWidget->pfCreateIndirect(paWidget, hParent, x0, y0, cb); /* Create child window */
		WM_ShowWindow(hChild);
		paWidget++;
		(*RMs).RM[i++] = hChild;
	}
	WM_SetFocusOnNextChild(hParent); /* Set the focus to the first child */
	WM_SendMessageNoPara(hParent, WM_INIT_DIALOG);
	return hParent;
}

