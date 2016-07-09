#pragma once

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "GUI.h"
#include "DIALOG.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"

typedef struct ResourceMap_struct {
	int Num;
	WM_HWIN RM[30];
} ResourceMap_struct;

#include "main.h"
#include "GUIUsual.h"
#include "GUIDisp.h"
#include "GUIDraw.h"
#include "GUINumPad.h"
#include "__UserGUI.h"

/*********************************************************************
*
*       Struct
*
**********************************************************************
*/
/*按键句柄-------------------------------------------------------*/
typedef struct {
	WM_HWIN ZoomPlus;
	WM_HWIN ZoomSub;
	WM_HWIN Measure;
	WM_HWIN NumPad;
	WM_HWIN Stop;
	WM_HWIN Reserve1;
	WM_HWIN Reserve2;
	WM_HWIN Reserve3;
} g_MAINBUTTON_struct;

/*主要显示句柄-------------------------------------------------------*/
typedef struct {
	WM_HWIN MainDlg;
	WM_HWIN AppendDlg;
	WM_HWIN hItemGraph_OSC;//全局图形句柄
} g_Disp_struct;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define DATAARRAYLENTGH 680//显示数据长度
#define SPEED           1250//延时速度

#define SCALE_V_Pos 35//坐标位置
#define SCALE_H_Pos 155

#define MEASURE_INFO_BEGIN_X 100//测试数据位置
#define MEASURE_INFO_BEGIN_Y 20

