#include "LCD_ILI9341_Graph.h"
#include "LCD_ILI9341.h"
/****************************************Copyright (c)****************************************************
**                                 ������ɽ�ҿƼ����޹�˾
**                                 ��      ����http://shan-yan.taobao.com
**                                 ΢      ����http://weibo.com/shanyantech
**                                 ��������Ⱥ��204669564
**								   							 Q        Q: 895838470
**--------------File Info---------------------------------------------------------------------------------
** File name:               LCD.c
** Descriptions:            ILI9341 SPI��������
**                          ��Ȩ���У�ʹ���뱣����
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


//������ɫ,������ɫ
uint16_t BSP_POINT_COLOR = 0x0000, BSP_BACK_COLOR = 0xFFFF;


//LCD������ʾ
void BSP_LCD_DisplayOn (void) {
	ili9341_WriteReg(0X29); //26��ɫ��ʾ����
}


//LCD�ر���ʾ
void BSP_LCD_DisplayOff (void) {
	ili9341_WriteReg(0X28);;//�ر���ʾ 
}


//���ù��λ��
//Xpos:������
//Ypos:������
__inline void BSP_LCD_SetCursor (uint16_t Xpos, uint16_t Ypos) {
	ili9341_WriteReg(LCD_COLUMN_ADDR);
	ili9341_WriteData(Xpos >> 8);
	ili9341_WriteData(0xFF & Xpos); //�趨X����

	ili9341_WriteReg(LCD_PAGE_ADDR);
	ili9341_WriteData(Ypos >> 8);
	ili9341_WriteData(0xFF & Ypos); //�趨Y����
}


//����
//x:0~239
//y:0~319
//POINT_COLOR:�˵����ɫ
void BSP_LCD_DrawPoint (uint16_t x, uint16_t y) {
	BSP_LCD_SetCursor(x, y);
	LCD_WriteRAM_Prepare();
	ili9341_WriteData(BSP_POINT_COLOR >> 8);
	ili9341_WriteData(BSP_POINT_COLOR);
}


//��������
//Color:Ҫ���������ɫ
void BSP_LCD_Clear (uint16_t Color) {
	uint32_t index = 0;
	BSP_LCD_SetCursor(0x00, 0x0000);//���ù��λ�� 
	LCD_WriteRAM_Prepare();
	for (index = 0; index < 320 * 240; index++) {
		ili9341_WriteData(Color >> 8);
		ili9341_WriteData(Color);
	}
}


//��ָ�����������ָ����ɫ
//�����С:
//  (xend-xsta)*(yend-ysta)
void BSP_LCD_Fill (uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color) {
	uint16_t i, j;
	uint16_t xlen = 0;
#if USE_HORIZONTAL ==1
	xlen = yend - ysta + 1;
	for (i = xsta; i <= xend; i++) {
		BSP_LCD_SetCursor (i, ysta);      //���ù��λ�� 
		LCD_WriteRAM_Prepare ();     //��ʼд��GRAM	  
		for (j = 0; j < xlen; j++) {
			ili9341_WriteData (color >> 8);
			ili9341_WriteData (color);
		}
	}
#else
	xlen = xend - xsta + 1;
	for (i = ysta; i <= yend; i++) {
		BSP_LCD_SetCursor(xsta, i); //���ù��λ�� 
		LCD_WriteRAM_Prepare(); //��ʼд��GRAM	  
		for (j = 0; j < xlen; j++) {
			ili9341_WriteData(color >> 8);
			ili9341_WriteData(color);
		}
	}
#endif 
}


//����
//x1,y1:�������
//x2,y2:�յ�����  
void BSP_LCD_DrawLine (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;

	delta_x = x2 - x1; //������������ 
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)incx = 1; //���õ������� 
	else if (delta_x == 0)incx = 0;//��ֱ�� 
	else {
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)incy = 1;
	else if (delta_y == 0)incy = 0;//ˮƽ�� 
	else {
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)distance = delta_x; //ѡȡ�������������� 
	else distance = delta_y;
	for (t = 0; t <= distance + 1; t++)//������� 
	{
		BSP_LCD_DrawPoint(uRow, uCol);//���� 
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

//������
void BSP_LCD_DrawRectangle (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	BSP_LCD_DrawLine(x1, y1, x2, y1);
	BSP_LCD_DrawLine(x1, y1, x1, y2);
	BSP_LCD_DrawLine(x1, y2, x2, y2);
	BSP_LCD_DrawLine(x2, y1, x2, y2);
}

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void BSP_Draw_Circle (uint16_t x0, uint16_t y0, uint8_t r) {
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1); //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
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
	while ((SPI5->SR & 1 << 1) == 0); //�ȴ���������
	SPI5->DR = 0x00;
	while ((SPI5->SR & 1 << 1) == 0); //�ȴ���������
	return SPI5->DR; //�����յ�������
}

