#include "GUIUsual.h"

/*-------------------------------------------------------*/
void MemDisp (int x, int y) {
	static GUI_ALLOC_DATATYPE MemFree, MemUsed;
	static int FontSizeY;//字体高度
	/*-------------------------------------------------------*/
	GUI_SetFont(&GUI_Font13_ASCII);//设置字体
	GUI_SetColor(GUI_RED);
	/*-------------------------------------------------------*/
	MemFree = GUI_ALLOC_GetNumFreeBytes();
	MemUsed = GUI_ALLOC_GetNumUsedBytes();
	if (MemFree < MemUsed)
	__NOP();
	FontSizeY = GUI_GetFontSizeY();//返回字体高度
	GUI_DispStringAt("Mem Used:", x, y);
	GUI_DispDecMin(MemUsed);
	GUI_DispStringAt("Mem Free:", x, y + FontSizeY);
	GUI_DispDecMin(MemFree);
}

/*-------------------------------------------------------*/
void WM_Paint_Button (WM_HWIN hObj) {
	//GUI_Exec();
	WM_Paint(hObj);
	ESP_SwapPage(&g_DispPage);
	WM_Paint(hObj);
	ESP_SwapPage(&g_DispPage);
}

