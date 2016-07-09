#ifndef __BASICGRAPHICS_H__
#define __BASICGRAPHICS_H__

#include "main.h"
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

void ESP_LCD_Fill (u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color);//在指定区域内填充指定颜色
void ESP_LCD_DrawPoint_big (u16 x_cur, u16 y_cur);//画一个大点
void ESP_LCD_DrawLine (u16 x1, u16 y1, u16 x2, u16 y2);////画线不指定颜色
void ESP_Draw_Circle (u16 x0, u16 y0, u8 r);////画矩形不指定颜色
void ESP_LCD_DrawRectangle (u16 x1, u16 y1, u16 x2, u16 y2); //在指定位置画一个指定大小的圆

void ESP_LCD_Draw_Vline (u16 x0, u16 y0, u16 len, u16 color); //画垂直线
void ESP_LCD_Draw_Hline (u16 x0, u16 y0, u16 len, u16 color); //画水平线
void ESP_LCD_Fill_Circle (u16 x0, u16 y0, u16 r, u16 color); //画实心圆
void ESP_LCD_Draw_Ellipse (u16 x0, u16 y0, u16 rx, u16 ry, u16 color); //画椭圆
void ESP_LCD_Fill_Ellipse (u16 x0, u16 y0, u16 rx, u16 ry, u16 color); //画实心椭圆

#endif

