#ifndef __BASICGRAPHICS_H__
#define __BASICGRAPHICS_H__

#include "main.h"
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

void ESP_LCD_Fill (u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color);//��ָ�����������ָ����ɫ
void ESP_LCD_DrawPoint_big (u16 x_cur, u16 y_cur);//��һ�����
void ESP_LCD_DrawLine (u16 x1, u16 y1, u16 x2, u16 y2);////���߲�ָ����ɫ
void ESP_Draw_Circle (u16 x0, u16 y0, u8 r);////�����β�ָ����ɫ
void ESP_LCD_DrawRectangle (u16 x1, u16 y1, u16 x2, u16 y2); //��ָ��λ�û�һ��ָ����С��Բ

void ESP_LCD_Draw_Vline (u16 x0, u16 y0, u16 len, u16 color); //����ֱ��
void ESP_LCD_Draw_Hline (u16 x0, u16 y0, u16 len, u16 color); //��ˮƽ��
void ESP_LCD_Fill_Circle (u16 x0, u16 y0, u16 r, u16 color); //��ʵ��Բ
void ESP_LCD_Draw_Ellipse (u16 x0, u16 y0, u16 rx, u16 ry, u16 color); //����Բ
void ESP_LCD_Fill_Ellipse (u16 x0, u16 y0, u16 rx, u16 ry, u16 color); //��ʵ����Բ

#endif

