#include "GUIDisp.h"
/*********************************************************************
*
*       extern data
*
**********************************************************************
*/
g_MAINBUTTON_struct g_MainButton = {0,0,0,0,0,0,0,0};
g_Disp_struct g_Disp = {0,0,0};
/*********************************************************************
*
*       global data
*
**********************************************************************
*/
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static GRAPH_SCALE_Handle g_hScaleV; // Handle of vertical scale
static GRAPH_SCALE_Handle g_hScaleH; // Handle of horizontal scale

static int GUI_Exec_State = 1; // 显存一的数据
//
// Dialog ressource（global）
//
const GUI_WIDGET_CREATE_INFO OSC_DialogCreate[] = {
	{WINDOW_CreateIndirect, 0, 0, 0, 0, 800, 480},//不可移动
	{BUTTON_CreateIndirect, "Zoom+", GUI_ID_BUTTON0, 692, 15, 105, 51, 0, 0},
	{BUTTON_CreateIndirect, "Zoom-", GUI_ID_BUTTON1, 692, 72, 105, 51, 0, 0},
	{BUTTON_CreateIndirect, "Measure", GUI_ID_BUTTON2, 692, 129, 105, 51, 0, 0},
	{BUTTON_CreateIndirect, "NumPad", GUI_ID_BUTTON3, 692, 186, 105, 51, 0, 0},
	{BUTTON_CreateIndirect, "Stop", GUI_ID_BUTTON4, 692, 243, 105, 51, 0, 0},
	{BUTTON_CreateIndirect, "Reserve", GUI_ID_BUTTON5, 692, 300, 105, 51, 0, 0},
	{BUTTON_CreateIndirect, "Reserve", GUI_ID_BUTTON6, 692, 357, 105, 51, 0, 0},
	{BUTTON_CreateIndirect, "Reserve", GUI_ID_BUTTON7, 692, 414, 105, 51, 0, 0},
	{GRAPH_CreateIndirect, 0, GUI_ID_GRAPH0, 2, 2, 685, 478},
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*****************************************************************
**      FunctionName:void OSC_PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void OSC_PaintDialog (WM_MESSAGE* pMsg) {
	WM_HWIN hWin = pMsg->hWin;

}

/*********************************************************************
*
*       初始化对话框
*
* Function description
*   This routine is called by the GRAPH object before anything is drawn
*   and after the last drawing operation.
*/
static void OSC_InitDialog (WM_MESSAGE* pMsg) {
	WM_HWIN hWin = pMsg->hWin;
	for (int i = 0; i < 8; i++)
		BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON0 + i), &GUI_Font16B_ASCII);
}

/*********************************************************************
*
*       _UserDraw
*
* Function description
*   This routine is called by the GRAPH object before anything is drawn
*   and after the last drawing operation.
*/
void Main_UserDraw (WM_HWIN hWin, int Stage) {
	if (Stage == GRAPH_DRAW_LAST) {
		static char ycText[] = "Amplitude ";//纵坐标单位说明
		static char xcText[] = "Time ";//横坐标单位说明
		static GUI_RECT RectY;
		static GUI_RECT RectX;
		static GUI_RECT RectInvalid;
		static int FontSizeY;//字体高度

		GUI_SetFont(&GUI_Font13_ASCII);//设置字体
		FontSizeY = GUI_GetFontSizeY();//返回字体高度
		WM_GetInsideRect(&RectY);//返回窗口尺寸
		WM_GetInsideRect(&RectX);
		WM_GetInvalidRect(hWin, &RectInvalid);//返回窗口尺寸减去边界尺寸后的坐标
		RectY.x1 = RectY.x0 + FontSizeY;
		RectX.y0 = SCALE_H_Pos + FontSizeY - 2;
		RectX.y1 = RectX.y0 + FontSizeY;
		GUI_SetColor(GUI_YELLOW);
		GUI_DispStringInRectEx(ycText, &RectY, GUI_TA_CENTER, strlen(ycText), GUI_ROTATE_CCW);//纵向显示（GUI_ROTATE_CCW），水平对齐（GUI_TA_HCENTER）
		GUI_DispStringInRectEx(xcText, &RectX, GUI_TA_HCENTER, strlen(xcText), GUI_ROTATE_0);//从左到右显示（GUI_ROTATE_0），水平对齐（GUI_TA_HCENTER）

		/*显示参数-------------------------------------------------------*/
		if (g_OSCInfo.MeasureStatu == eOpen)
			OSC_MeasureInfoSwitch(eOpen);
		/*内存监视-------------------------------------------------------*/
#ifdef MEM_DEBUG__
		MemDisp(200, 200);
#endif
		/*-------------------------------------------------------*/
	}
}


/*********************************************************************
*
*       _cbCallback
*
* Function description
*   Callback function of the dialog
*/
void Main_cbCallback (WM_MESSAGE* pMsg) {
	int NCode, Id;
	WM_HWIN hDlg;
	WM_HWIN hItem;

	hDlg = pMsg->hWin;
	switch (pMsg->MsgId) {
		case WM_INIT_DIALOG:
			OSC_InitDialog(pMsg);
			hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
			g_Disp.hItemGraph_OSC = hItem;
			//
			// Set graph attributes
			//
			MemDisp(200, 200);
			GRAPH_SetGridDistY(hItem, 25);
			GRAPH_SetGridVis(hItem, 1);
			GRAPH_SetGridFixedX(hItem, 1);
			GRAPH_SetUserDraw(hItem, Main_UserDraw);
			//
			// Create and add vertical scale
			//
			g_hScaleV = GRAPH_SCALE_Create(SCALE_V_Pos, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 25);
			GRAPH_SCALE_SetTextColor(g_hScaleV, GUI_YELLOW);
			GRAPH_AttachScale(hItem, g_hScaleV);
			//
			// Create and add horizontal scale
			//
			g_hScaleH = GRAPH_SCALE_Create(SCALE_H_Pos, GUI_TA_HCENTER, GRAPH_SCALE_CF_HORIZONTAL, 50);
			GRAPH_SCALE_SetTextColor(g_hScaleH, GUI_DARKGREEN);
			GRAPH_AttachScale(hItem, g_hScaleH);
		case WM_PAINT:
			OSC_PaintDialog(pMsg);
			break;
		case WM_KEY:
			switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) {
				case GUI_KEY_ESCAPE:
					GUI_EndDialog(hDlg, 1);
					break;
				case GUI_KEY_ENTER:
					GUI_EndDialog(hDlg, 0);
					break;
			}
			break;
		case WM_NOTIFY_PARENT:
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch (Id) {
				case GUI_ID_OK:
					if (NCode == WM_NOTIFICATION_RELEASED)
						GUI_EndDialog(hDlg, 0);
					break;
				case GUI_ID_CANCEL:
					if (NCode == WM_NOTIFICATION_RELEASED)
						GUI_EndDialog(hDlg, 0);
					break;
				case GUI_ID_BUTTON0: //Zoom+
					if (NCode == WM_NOTIFICATION_RELEASED)
						g_OSCInfo.Res_rat = Safe_Return (g_OSCInfo.Res_rat + 1, eReslt_rat_Begin, eReslt_rat_End) ;
					break;
				case GUI_ID_BUTTON1: //Zoom-
					if (NCode == WM_NOTIFICATION_RELEASED)
						g_OSCInfo.Res_rat = Safe_Return (g_OSCInfo.Res_rat - 1, eReslt_rat_Begin, eReslt_rat_End) ;
					break;
				case GUI_ID_BUTTON2: //Measure
					if (NCode == WM_NOTIFICATION_RELEASED)
						if (g_OSCInfo.MeasureStatu == eClose) {//打开测量数据
							g_OSCInfo.MeasureStatu = eOpen;
							OSC_MeasureInfoSwitch(eOpen);
						}
						else {//关闭测量数据
							g_OSCInfo.MeasureStatu = eClose;
							OSC_MeasureInfoSwitch(eClose);
						}
					break;
				case GUI_ID_BUTTON3: //NumPad
					if (NCode == WM_NOTIFICATION_RELEASED)
						if (g_GUIControl.InputDialogState == eClose) {//打开NumPad
							g_GUIControl.InputDialogState = eOpen;
							//WM_ShowWindow(g_Disp.AppendDlg);
							RMSwitch(eOpen, NumPad_RMs);
						}
						else {//关闭NumPad
							g_GUIControl.InputDialogState = eClose;
							//WM_HideWindow(g_Disp.AppendDlg);
							RMSwitch(eClose, NumPad_RMs);
						}
					break;
				case GUI_ID_BUTTON4: //Stop
					if (NCode == WM_NOTIFICATION_RELEASED)
						if (g_GUIControl.DispState == eClose) {//开始
							g_GUIControl.DispState = eOpen;
							BUTTON_SetText(g_MainButton.Stop, "Stop");
							BUTTON_SetBkColor(g_MainButton.Stop, BUTTON_CI_PRESSED, GUI_BLUE);
							BUTTON_SetBkColor(g_MainButton.Stop, BUTTON_CI_UNPRESSED, GUI_BLUE);
						}
						else {//暂停
							g_GUIControl.DispState = eClose;
							BUTTON_SetText(g_MainButton.Stop, "Continue");
							BUTTON_SetBkColor(g_MainButton.Stop, BUTTON_CI_PRESSED, GUI_RED);
							BUTTON_SetBkColor(g_MainButton.Stop, BUTTON_CI_UNPRESSED, GUI_RED);
						}
					break;
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}


WM_HWIN Main_CreateWindow (void) {
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(OSC_DialogCreate, GUI_COUNTOF (OSC_DialogCreate), Main_cbCallback, WM_HBKWIN, 0, 0);
	return hWin;
}

