#include "System.h" 

//"系统" 常用功能函数集-头文件
extern g_Disp_struct g_Disp;

/*  ---------------------------------------------------------*/
void System_Initial_OSC (void) {
	/*tft初始化-------------------------------------------------------*/
	BSP_LCD_Init();
	ESP_LCD_Init();
	/*触摸屏初始化-------------------------------------------------------*/
	ESP_TS_Init();
	BSP_TS_Init(240, 320);
	/* 清屏 ---------------------------------------------------------*/
#ifdef SINGLE_PAGE__
	ESP_ShowPage(ePage_Num_1);
	ESP_WritePage(ePage_Num_1);
	ESP_LCD_Clear(BLACK);
#else
	ESP_SwapPage(&g_DispPage);
	ESP_LCD_Clear(BLACK);
	ESP_SwapPage(&g_DispPage);
	ESP_LCD_Clear(BLACK);
#endif
	/*-------------------------------------------------------*/
	//WM_MULTIBUF_Enable(1);//启动多重缓存
	GUI_Init();//GUI初始化
	ESP_MainTask();//界面初始化
	BSP_MainTask();//界面初始化
}

void BSP_Background (void) {
	/*BSP-------------------------------------------------------*/
	GUI_SelectLayer(eLAYER_BSP);//选择绘图层
	BSP_Pointer_Update();
	GUI_Exec();
	//WM_Update(g_Disp.AppendDlg);
	/*ESP-------------------------------------------------------*/
	GUI_SelectLayer(eLAYER_ESP);//选择绘图层
	ESP_Pointer_Update();
	GUI_Exec();
	/*-------------------------------------------------------*/
#ifndef SINGLE_PAGE__
	ESP_SwapPage(&g_DispPage);//换页
#endif
}

