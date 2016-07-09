#include "LCD_ILI9341_Graph.h"
#include "LCD_ILI9341.h"
/****************************************Copyright (c)****************************************************
**                                 深圳市山岩科技有限公司
**                                 淘      宝：http://shan-yan.taobao.com
**                                 微      博：http://weibo.com/shanyantech
**                                 技术交流群：204669564
**								   							 Q        Q: 895838470
**--------------File Info---------------------------------------------------------------------------------
** File name:               LCD.c
** Descriptions:            ILI9341 SPI驱动程序
**                          版权所有，使用请保留！
**--------------------------------------------------------------------------------------------------------
** Created by:              shanyan
** Created date:            2013-12-19
** Version:                 v0.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/


//画笔颜色,背景颜色
uint16_t BSP_POINT_COLOR = 0x0000, BSP_BACK_COLOR = 0xFFFF;


//LCD开启显示
void BSP_LCD_DisplayOn (void) {
	ili9341_WriteReg(0X29); //26万色显示开启
}


//LCD关闭显示
void BSP_LCD_DisplayOff (void) {
	ili9341_WriteReg(0X28);;//关闭显示 
}


//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
__inline void BSP_LCD_SetCursor (uint16_t Xpos, uint16_t Ypos) {
	ili9341_WriteReg(LCD_COLUMN_ADDR);
	ili9341_WriteData(Xpos >> 8);
	ili9341_WriteData(0xFF & Xpos); //设定X坐标

	ili9341_WriteReg(LCD_PAGE_ADDR);
	ili9341_WriteData(Ypos >> 8);
	ili9341_WriteData(0xFF & Ypos); //设定Y坐标
}


//画点
//x:0~239
//y:0~319
//POINT_COLOR:此点的颜色
void BSP_LCD_DrawPoint (uint16_t x, uint16_t y) {
	BSP_LCD_SetCursor(x, y);
	LCD_WriteRAM_Prepare();
	ili9341_WriteData(BSP_POINT_COLOR >> 8);
	ili9341_WriteData(BSP_POINT_COLOR);
}


//清屏函数
//Color:要清屏的填充色
void BSP_LCD_Clear (uint16_t Color) {
	uint32_t index = 0;
	BSP_LCD_SetCursor(0x00, 0x0000);//设置光标位置 
	LCD_WriteRAM_Prepare();
	for (index = 0; index < 320 * 240; index++) {
		ili9341_WriteData(Color >> 8);
		ili9341_WriteData(Color);
	}
}


//在指定区域内填充指定颜色
//区域大小:
//  (xend-xsta)*(yend-ysta)
void BSP_LCD_Fill (uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color) {
	uint16_t i, j;
	uint16_t xlen = 0;
#if USE_HORIZONTAL ==1
	xlen = yend - ysta + 1;
	for (i = xsta; i <= xend; i++) {
		BSP_LCD_SetCursor (i, ysta);      //设置光标位置 
		LCD_WriteRAM_Prepare ();     //开始写入GRAM	  
		for (j = 0; j < xlen; j++) {
			ili9341_WriteData (color >> 8);
			ili9341_WriteData (color);
		}
	}
#else
	xlen = xend - xsta + 1;
	for (i = ysta; i <= yend; i++) {
		BSP_LCD_SetCursor(xsta, i); //设置光标位置 
		LCD_WriteRAM_Prepare(); //开始写入GRAM	  
		for (j = 0; j < xlen; j++) {
			ili9341_WriteData(color >> 8);
			ili9341_WriteData(color);
		}
	}
#endif 
}


//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void BSP_LCD_DrawLine (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;

	delta_x = x2 - x1; //计算坐标增量 
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)incx = 1; //设置单步方向 
	else if (delta_x == 0)incx = 0;//垂直线 
	else {
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)incy = 1;
	else if (delta_y == 0)incy = 0;//水平线 
	else {
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴 
	else distance = delta_y;
	for (t = 0; t <= distance + 1; t++)//画线输出 
	{
		BSP_LCD_DrawPoint(uRow, uCol);//画点 
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance) {
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance) {
			yerr -= distance;
			uCol += incy;
		}
	}
}

//画矩形
void BSP_LCD_DrawRectangle (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	BSP_LCD_DrawLine(x1, y1, x2, y1);
	BSP_LCD_DrawLine(x1, y1, x1, y2);
	BSP_LCD_DrawLine(x1, y2, x2, y2);
	BSP_LCD_DrawLine(x2, y1, x2, y2);
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void BSP_Draw_Circle (uint16_t x0, uint16_t y0, uint8_t r) {
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1); //判断下个点位置的标志
	while (a <= b) {
		BSP_LCD_DrawPoint(x0 - b, y0 - a); //3           
		BSP_LCD_DrawPoint(x0 + b, y0 - a); //0           
		BSP_LCD_DrawPoint(x0 - a, y0 + b); //1       
		BSP_LCD_DrawPoint(x0 - b, y0 - a); //7           
		BSP_LCD_DrawPoint(x0 - a, y0 - b); //2             
		BSP_LCD_DrawPoint(x0 + b, y0 + a); //4               
		BSP_LCD_DrawPoint(x0 + a, y0 - b); //5
		BSP_LCD_DrawPoint(x0 + a, y0 + b); //6 
		BSP_LCD_DrawPoint(x0 - b, y0 + a);
		a++;
		//使用Bresenham算法画圆     
		if (di < 0)di += 4 * a + 6;
		else {
			di += 10 + 4 * (a - b);
			b--;
		}
		BSP_LCD_DrawPoint(x0 + a, y0 + b);
	}
}

void BSP_Address_Set (u16 x1, u16 y1, u16 x2, u16 y2) {
	ili9341_WriteReg(0x2a);
	ili9341_WriteData(x1 >> 8);
	ili9341_WriteData(x1);
	ili9341_WriteData(x2 >> 8);
	ili9341_WriteData(x2);

	ili9341_WriteReg(0x2b);
	ili9341_WriteData(y1 >> 8);
	ili9341_WriteData(y1);
	ili9341_WriteData(y2 >> 8);
	ili9341_WriteData(y2);

	ili9341_WriteReg(0x2c);
}

u16 BSP_LCD_READ_POINT (u16 x,u16 y) {
	BSP_LCD_SetCursor(x, y);
	ili9341_WriteReg(0x2e);
	BSP_LCD_READ_DATE();
	BSP_LCD_READ_DATE();
	return (((BSP_LCD_READ_DATE() & 0X007c) << 9) | (BSP_LCD_READ_DATE() << 4) | (BSP_LCD_READ_DATE() >> 2));

}

u8 BSP_LCD_READ_DATE (void) {
	LCD_WRX_HIGH ();
	SPI5->DR = 0xff;
	while ((SPI5->SR & 1 << 1) == 0); //等待发送区空
	SPI5->DR = 0x00;
	while ((SPI5->SR & 1 << 1) == 0); //等待发送区空
	return SPI5->DR; //返回收到的数据
}

