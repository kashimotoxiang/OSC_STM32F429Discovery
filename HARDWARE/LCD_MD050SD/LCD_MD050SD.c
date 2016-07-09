#include "LCD_MD050SD.h"
u16 BACK_COLOR, POINT_COLOR; //背景色，画笔色

void ESP_LCD_WriteReg (u16 LCD_Reg, u16 LCD_RegValue) {
	LCD_WR_REG(LCD_Reg) 

		;
	LCD_WR_DATA(LCD_RegValue) 

		;
}

//LCDaê
void ESP_LCD_DisplayOn (void) {
	ESP_LCD_WriteReg(0x01, 16); //26íòéêa
}

//LCD1±ê
void ESP_LCD_DisplayOff (void) {
	ESP_LCD_WriteReg(0x000B, 0x0001);//1±ê
}

//éèeêμ・óáêμ・
void ESP_Address_set (u16 x1, u16 y1, u16 x2, u16 y2) {
	ESP_LCD_WriteReg(0x0002, y1);
	ESP_LCD_WriteReg(0x0003, x1);
	ESP_LCD_WriteReg(0x0006, y2);
	ESP_LCD_WriteReg(0x0007, x2);
	LCD_WR_REG(0x000f);
}

/*  ---------------------------------------------------------*/
void ESP_LCD_Init (void) {
	LCD_RD_SET ;
	LCD_WR_SET ;
	LCD_REST_CLR ;
	delay_ms(50); //′′|±￡¤òaóD1msòé
	LCD_REST_SET ;
	delay_ms(50); //′′|±￡¤òaóD1msòé

	LCD_CS_CLR ; //′òaê1ü
	ESP_LCD_DisplayOn();
}

//---------------------------------------------------------------------//
//清屏函数
//POINT_COLOR:要清屏的填充色
void ESP_LCD_Clear (u16 POINT_COLOR) {
	u16 i, j;
	ESP_Address_set(0, 0, LCD_W_ - 1, LCD_H_ - 1);
	for (i = 0; i < LCD_W_; i++) {
		for (j = 0; j < LCD_H_; j++) {
			LCD_WR_DATA(POINT_COLOR) 

				;
		}
	}
}

//在指定位置显示一个汉字(32*33大小)
//dcolor为内容颜色，gbcolor为背静颜色
//void showhanzi(unsigned int x_cur,unsigned int y_cur,unsigned char index,u16 POINT_COLOR)
//{
//	unsigned char i,j;
//	unsigned char *temp;
//        temp=&(hanzi[0]);
//        Address_set(x_cur,y_cur,x_cur+31,y_cur+31); //设置区域
//	temp+=index*128;
//	for(j=0;j<128;j++)
//	{
//		for(i=0;i<8;i++)
//		{
//		 	if((*temp&(1<<i))!=0)
//			{
//				LCD_WR_DATA(POINT_COLOR);
//			}
//			else
//			{
//				LCD_WR_DATA(BACK_COLOR);
//			}
//		}
//		temp++;
//	 }
//}


//在指定位置显示一个字符

//num:要显示的字符:" "--->"~"
//mode:叠加方式(1)还是非叠加方式(0)
//在指定位置显示一个字符

//num:要显示的字符:" "--->"~"

//mode:叠加方式(1)还是非叠加方式(0)


//void LCD_Show_Char (u16 x_cur, u16 y_cur, u8 num, u8 mode) {
//	u8 temp;
//	u8 pos, t;
//	u16 colortemp = POINT_COLOR;
//	if (x_cur > LCD_W_ - 16 || y_cur > LCD_H_ - 16)return;
//	//设置窗口
//	num = num - ' ';//得到偏移后的值
//	Address_set(x_cur, y_cur, x_cur + 8 - 1, y_cur + 16 - 1); //设置光标位置
//	for (pos = 0; pos < 16; pos++) {
//		temp = asc2_1608[num][pos];
//		for (t = 0; t < 8; t++) {
//			if (temp & 0x01)
//				POINT_COLOR = colortemp;
//			else
//				POINT_COLOR = BACK_COLOR;
//			LCD_DrawPoint(x_cur + t, y_cur + pos);
//			temp >>= 1;
//		}
//	}

//	POINT_COLOR = colortemp;
//}

//m^n函数
u32 ESP_mypow (u8 m, u8 n) {
	u32 result = 1;
	while (n--)result *= m;
	return result;
}

//显示2个数字
//x_cur,y_cur :起点坐标
//len :数字的位数
//POINT_COLOR:颜色
//num:数值(0~4294967295);
void ESP_LCD_Show_Num (u16 x_cur, u16 y_cur, u32 num, u8 len) {
	u8 t, temp;
	u8 enshow = 0;
	num = (u16)num;
	for (t = 0; t < len; t++) {
		temp = (num / ESP_mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1)) {
			if (temp == 0) {
				//LCD_Show_Char(x_cur + 8 * t, y_cur, ' ', 0);
				continue;
			}
			else enshow = 1;
		}
		//LCD_Show_Char(x_cur + 8 * t, y_cur, temp + 48, 0);
	}
}

//显示字符串
//x_cur,y_cur:起点坐标
//*p:字符串起始地址
//用16字体
void ESP_LCD_ShowString (u16 x_cur, u16 y_cur, char* p) {
	while (*p != '\0') {
		if (x_cur > LCD_W_ - 16) {
			x_cur = 0;
			y_cur += 16;
		}
		if (y_cur > LCD_H_ - 16) {
			y_cur = x_cur = 0;
			ESP_LCD_Clear(RED);
		}
		//LCD_Show_Char(x_cur, y_cur, *p, 0);
		x_cur += 8;
		p++;
	}
}

//---------------------------------------------------------------------//
void ESP_WritePage (unsigned char index)//设置当前操作页，上电默认为0
{
	LCD_WR_REG(0x05) 

		;
	LCD_WR_DATA(index) 

		;
}

void ESP_ShowPage (unsigned char index)//设置当前显示页，上电默认为0
{
	LCD_WR_REG(0x04) 

		;
	LCD_WR_DATA(index) 

		;
}

/*===================================================================*/
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u16 ESP_LCD_ReadPoint (u16 x, u16 y) {
	if (x >= eLCDPar_width || y >= eLCDPar_height)
		return 0; //超过了范围,直接返回
	ESP_Address_set(x, y, x, y);
	return LCD_RD_DATA() 

		; //这几种IC直接返回颜色值
}

/*交换页码---------------------------------------------------------*/
void ESP_SwapPage (Page_struct* a) {
#ifndef SINGLE_PAGE__
	ESP_ShowPage((*a).Two);
	ESP_WritePage((*a).One);
	(*a).Temp = (*a).One;
	(*a).One = (*a).Two;
	(*a).Two = (*a).Temp;
	return;
#endif
}

