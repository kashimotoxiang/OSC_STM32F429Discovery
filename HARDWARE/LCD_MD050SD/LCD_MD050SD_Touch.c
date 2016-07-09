#include "LCD_MD050SD_Touch.h"
#define delay_ns_ 8

#define READ_TIMES 6 //????
#define LOST_VAL 1	  //???

u16 vx = 4421, vy = 6078;
u16 chx = 247, chy = 3388;
Touch_Data_Struct tp_pixad, ESP_TS; //???????AD?,?????????
//------------------------------------------------------------------------------------------//
inline u8 tpstate (void) {
	return T_IRQ_Dect ;
}

//------------------------------------------------------------------------------------------//
void ESP_TS_Init (void) //SPI??
{
	T_CS_High ;
	T_CLK_High ;
	T_DIN_High ;
	T_CLK_High ;
}

//------------------------------------------------------------------------------------------//
void WriteCharTo7843 (unsigned char num) //SPI???
{
	static unsigned char count = 0;
	count = 0;
	T_CLK_Low ;
	for (count = 0; count < 8; count++) {
		if (num & 0x80)
		T_DIN_High ;
		else
		T_DIN_Low ;
		num <<= 1;
		T_CLK_Low ;
		delay_25ns(delay_ns_); //?????
		T_CLK_High ;
		delay_25ns(delay_ns_);
	}
}

//------------------------------------------------------------------------------------------//
u16 ReadFromCharFrom7843 (void) //SPI ???
{
	static u8 count = 0;
	static u16 Num = 0;
	count = 0;
	Num = 0;
	for (count = 0; count < 12; count++) {
		Num <<= 1;
		T_CLK_High ;
		delay_25ns(delay_ns_); //?????
		T_CLK_Low ;
		delay_25ns(delay_ns_);
		if (
			T_DOUT_Dect) {
			Num |= 1;
		}
	}

	return (Num);
}

//------------------------------------------------------------------------------------------//
//?7846/7843/XPT2046/UH7843/UH7846??adc?	  0x90=y_cur   0xd0-x_cur
u16 ADS_Read_AD (unsigned char CMD) {
	static u16 l;
	T_CS_Low ;
	WriteCharTo7843(CMD); //???????????x_cur?? ????????
	T_CLK_High ;
	delay_25ns(delay_ns_);
	T_CLK_Low ;
	delay_25ns(delay_ns_);
	l = ReadFromCharFrom7843();
	return l;
}

//------------------------------------------------------------------------------------------//
//???????
//????READ_TIMES???,?????????,
//?????????LOST_VAL??,????
u16 ADS_Read_XY (u8 xy) {
	static u16 i, j;
	static u16 buf[READ_TIMES ];
	static u16 sum = 0;
	static u16 temp;
	sum = 0;
	for (i = 0; i < READ_TIMES; i++) {
		buf[i] = ADS_Read_AD(xy);
	}
	for (i = 0; i < READ_TIMES - 1; i++) //??
	{
		for (j = i + 1; j < READ_TIMES; j++) {
			if (buf[i] > buf[j])//????
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	sum = 0;
	for (i = LOST_VAL; i < READ_TIMES - LOST_VAL; i++)sum += buf[i];
	temp = sum / (READ_TIMES - 2 * LOST_VAL);
	return temp;
}

//------------------------------------------------------------------------------------------//
//????????
//???????100.
u8 Read_ADS (u16* x_cur, u16* y_cur) {
	static u16 xtemp, ytemp;
	xtemp = ADS_Read_XY(CMD_RDX);
	ytemp = ADS_Read_XY(CMD_RDY);
	if (xtemp < 100 || ytemp < 100)return 0;//????
	*x_cur = xtemp;
	*y_cur = ytemp;
	return 1;//????
}

//------------------------------------------------------------------------------------------//
//2???ADS7846,????2????AD?,???????????
//50,????,???????,??????.
//???????????
#define ERR_RANGE 20 //????
u8 Read_ADS2 (u16* x_cur, u16* y_cur) {
	static u16 x1, y1;
	static u16 x2, y2;
	static u8 flag;
	flag = Read_ADS(&x1, &y1);
	if (flag == 0)return (0);
	flag = Read_ADS(&x2, &y2);
	if (flag == 0)return (0);
	if (((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE))//???????+-ERR_RANGE?
		&& ((y2 <= y1 && y1 < y2 + ERR_RANGE) || (y1 <= y2 && y2 < y1 + ERR_RANGE))) {
		*x_cur = (x1 + x2) >> 1;
		*y_cur = (y1 + y2) >> 1;
		return 1;
	}
	else return 0;
}

//------------------------------------------------------------------------------------------//
//????????,???????
u8 Read_TP_Once (void) {
	static u8 re = 0;
	static u16 x1, y1;
	static u16 x2, y2;
	re = 0;
	while (re == 0) {
		while (!Read_ADS2(&x1, &y1));
		delay_ms(10);
		while (!Read_ADS2(&x2, &y2));
		if ((x2 <= x1 && x1 < x2 + 3) || (x1 <= x2 && x2 < x1 + 3)) {
			tp_pixad.xc = (x1 + x2) / 2;
			tp_pixad.yc = (y1 + y2) / 2;
			re = 1;
		}
	}
	return re;
}

//------------------------------------------------------------------------------------------//
//?LCD???????
//??????
//??????
void ESP_Drow_Touch_Point (u16 x_cur, u16 y_cur) {
	ESP_LCD_DrawLine(x_cur - 12, y_cur, x_cur + 13, y_cur);//??
	ESP_LCD_DrawLine(x_cur, y_cur - 12, x_cur, y_cur + 13);//??
	ESP_LCD_DrawPoint(x_cur + 1, y_cur + 1);
	ESP_LCD_DrawPoint(x_cur - 1, y_cur + 1);
	ESP_LCD_DrawPoint(x_cur + 1, y_cur - 1);
	ESP_LCD_DrawPoint(x_cur - 1, y_cur - 1);
	//	Draw_Circle(x_cur,y_cur,6);//????
}

//------------------------------------------------------------------------------------------//
//???????
//????????
#define tp_pianyi 80   //???????
#define tp_xiaozhun 2000   //????

void Touch_Adjust (void) {
	float vx1, vx2, vy1, vy2; //????,????1000???????AD????????
	u16 chx1, chx2, chy1, chy2;//????????0??AD???
	u16 lx, ly;
	struct tp_pixu32_ p[4];
	u8 cnt = 0;
	cnt = 0;
	POINT_COLOR = BLUE ;
	BACK_COLOR = WHITE;
	ESP_LCD_Clear(WHITE);//??
	POINT_COLOR = RED;//??
	ESP_LCD_Clear(WHITE);//??
	ESP_Drow_Touch_Point(tp_pianyi, tp_pianyi);//??1
	while (1) {
		if (T_IRQ_Dect)//?????
		{
			if (Read_TP_Once())//???????
			{
				p[cnt].xc = tp_pixad.xc;
				p[cnt].yc = tp_pixad.yc;
				cnt++;
			}
			switch (cnt) {
				case 1:
					ESP_LCD_Clear(WHITE);//??
					while (!T_IRQ_Dect) //????
					{
					}
					ESP_Drow_Touch_Point(LCD_W_ - tp_pianyi - 1, tp_pianyi);//??2
					break;
				case 2:
					ESP_LCD_Clear(WHITE);//??
					while (!T_IRQ_Dect) //????
					{
					}
					ESP_Drow_Touch_Point(tp_pianyi, LCD_H_ - tp_pianyi - 1);//??3
					break;
				case 3:
					ESP_LCD_Clear(WHITE);//??
					while (!T_IRQ_Dect) //????
					{
					}
					ESP_Drow_Touch_Point(LCD_W_ - tp_pianyi - 1, LCD_H_ - tp_pianyi - 1);//??4
					break;
				case 4: //?????????
					ESP_LCD_Clear(WHITE);//??
					while (!T_IRQ_Dect) //????
					{
					}
					vx1 = p[1].xc > p[0].xc ? (p[1].xc - p[0].xc + 1) * 1000 / (LCD_W_ - tp_pianyi - tp_pianyi) : (p[0].xc - p[1].xc - 1) * 1000 / (LCD_W_ - tp_pianyi - tp_pianyi);
					chx1 = p[1].xc > p[0].xc ? p[0].xc - (vx1 * tp_pianyi) / 1000 : p[0].xc + (vx1 * tp_pianyi) / 1000;
					vy1 = p[2].yc > p[0].yc ? (p[2].yc - p[0].yc - 1) * 1000 / (LCD_H_ - tp_pianyi - tp_pianyi) : (p[0].yc - p[2].yc - 1) * 1000 / (LCD_H_ - tp_pianyi - tp_pianyi);
					chy1 = p[2].yc > p[0].yc ? p[0].yc - (vy1 * tp_pianyi) / 1000 : p[0].yc + (vy1 * tp_pianyi) / 1000;

					vx2 = p[3].xc > p[2].xc ? (p[3].xc - p[2].xc + 1) * 1000 / (LCD_W_ - tp_pianyi - tp_pianyi) : (p[2].xc - p[3].xc - 1) * 1000 / (LCD_W_ - tp_pianyi - tp_pianyi);
					chx2 = p[3].xc > p[2].xc ? p[2].xc - (vx2 * tp_pianyi) / 1000 : p[2].xc + (vx2 * tp_pianyi) / 1000;
					vy2 = p[3].yc > p[1].yc ? (p[3].yc - p[1].yc - 1) * 1000 / (LCD_H_ - tp_pianyi - tp_pianyi) : (p[1].yc - p[3].yc - 1) * 1000 / (LCD_H_ - tp_pianyi - tp_pianyi);
					chy2 = p[3].yc > p[1].yc ? p[1].yc - (vy2 * tp_pianyi) / 1000 : p[1].yc + (vy2 * tp_pianyi) / 1000;

					if ((vx1 > vx2 && vx1 > vx2 + tp_xiaozhun) || (vx1 < vx2 && vx1 < vx2 - tp_xiaozhun) || (vy1 > vy2 && vy1 > vy2 + tp_xiaozhun) || (vy1 < vy2 && vy1 < vy2 - tp_xiaozhun)) {
						cnt = 0;
						ESP_LCD_Clear(WHITE);//??
						ESP_Drow_Touch_Point(tp_pianyi, tp_pianyi);//??1
						continue;
					}
					vx = (vx1 + vx2) / 2;
					vy = (vy1 + vy2) / 2;
					chx = (chx1 + chx2) / 2;
					chy = (chy1 + chy2) / 2;

					//??????
					ESP_LCD_Clear(WHITE);//??
					POINT_COLOR = YELLOW;
					BACK_COLOR = BLUE ;

					lx = 0;
					ly = 50;
					ESP_LCD_ShowString(lx, ly, "VX1:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, vx1, 4);
					lx = 0;
					ly += 20;
					ESP_LCD_ShowString(lx, ly, "Vy1:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, vy1, 4);
					lx = 0;
					ly += 20;
					ESP_LCD_ShowString(lx, ly, "CHX1:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, chx1, 4);
					lx = 0;
					ly += 20;
					ESP_LCD_ShowString(lx, ly, "CHY1:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, chy1, 4);

					lx = 100;
					ly = 50;
					ESP_LCD_ShowString(lx, ly, "VX2:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, vx2, 4);
					lx = 100;
					ly += 20;
					ESP_LCD_ShowString(lx, ly, "Vy2:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, vy2, 4);
					lx = 100;
					ly += 20;
					ESP_LCD_ShowString(lx, ly, "CHX2:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, chx2, 4);
					lx = 100;
					ly += 20;
					ESP_LCD_ShowString(lx, ly, "CHY2:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, chy2, 4);

					lx = 50;
					ly = 150;
					ESP_LCD_ShowString(lx, ly, "VX:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, vx, 4);
					lx = 50;
					ly += 20;
					ESP_LCD_ShowString(lx, ly, "Vy:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, vy, 4);
					lx = 50;
					ly += 20;
					ESP_LCD_ShowString(lx, ly, "CHX:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, chx, 4);
					lx = 50;
					ly += 20;
					ESP_LCD_ShowString(lx, ly, "CHY:");
					lx += 40;
					ESP_LCD_Show_Num(lx, ly, chy, 4);

					lx = 30;
					ly += 30;
					ESP_LCD_ShowString(lx, ly, "Adjust OK!  Touch Anywhere To Continue");
					Read_TP_Once(); //????????

					ESP_LCD_Clear(WHITE);//??
					return;//????
			}
		}
	}
}

//------------------------------------------------------------------------------------------//
void point (void) //????
{
	static double t = 0;
	while (1) {
		if (T_IRQ_Dect == 0) {
			t = 0;
			if (ESP_Convert_Pos()) //?????
			{
				POINT_COLOR = YELLOW;
				ESP_LCD_ShowString(10, 250, "x_cur:");
				ESP_LCD_Show_Num(30, 250, tp_pixad.xc, 4);
				ESP_LCD_ShowString(180, 250, "y_cur:");
				ESP_LCD_Show_Num(200, 250, tp_pixad.yc, 4);
				ESP_LCD_DrawPoint_big(ESP_TS.xc, ESP_TS.yc);
			}
		}
		else {
			t++;
			if (t > 65000) {
				return;
			}
		}
	}
}

/*------------------------触摸屏配置（SPI2）-------------------------------*/
u16 ESP_TP_Read_XOY (u8 xy) {
	static u16 i, j;
	static u16 buf[READ_TIMES ];
	static u16 sum = 0;
	static u16 temp;
	for (i = 0; i < READ_TIMES; i++)buf[i] = ADS_Read_XY(xy);
	for (i = 0; i < READ_TIMES - 1; i++) {
		for (j = i + 1; j < READ_TIMES; j++) {
			if (buf[i] > buf[j]) {
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	sum = 0;
	for (i = LOST_VAL; i < READ_TIMES - LOST_VAL; i++)sum += buf[i];
	temp = sum / (READ_TIMES - 2 * LOST_VAL);
	return temp;
}


/*-------------------------------------------------------*/
/**
* @brief:
* @param:
* @note:返回0触摸失败，返回1触摸成功
*/
u8 ESP_Convert_Pos (void) {
	static u8 state;
	state = 0;
	if (!T_IRQ_Dect) {//检测硬件触摸
		if (Read_ADS2(&tp_pixad.xc, &tp_pixad.yc)) {//判断触摸数据是否正常
			state = 1;
			ESP_TS.xc = tp_pixad.xc > chx ? ((u32)tp_pixad.xc - (u32)chx) * 1000 / vx : ((u32)chx - (u32)tp_pixad.xc) * 1000 / vx;
			ESP_TS.yc = tp_pixad.yc > chy ? ((u32)tp_pixad.yc - (u32)chy) * 1000 / vy : ((u32)chy - (u32)tp_pixad.yc) * 1000 / vy;
			return state;
		}
	}
	ESP_TS.xc = 0;
	ESP_TS.yc = 0;
	return state;
}

/*-------------------------------------------------------*/
/**
* @brief:
* @param:
* @note:单点触摸时touchDetected为0 1
*/
void ESP_Pointer_Update (void) {
	static GUI_PID_STATE TS_State = {0, 0, 0, eLAYER_ESP};
	static uint16_t xDiff, yDiff;//最终装载的点
	/*-------------------------------------------------------*/
	ESP_TS.TouchDetected = ESP_Convert_Pos();//判断是否有触摸//获取当前触摸数据 tp.xc & tp.yc

	/*不在屏幕范围内-------------------------------------------------------*/
	if ((ESP_TS.xc >= LCD_GetXSize()) || (ESP_TS.yc >= LCD_GetYSize())) {
		ESP_TS.xc = 0;
		ESP_TS.yc = 0;
		ESP_TS.TouchDetected = 0;
	}

	/*触摸滤波防抖-------------------------------------------------------*/
	xDiff = (TS_State.x > ESP_TS.xc) ? (TS_State.x - ESP_TS.xc) : (ESP_TS.xc - TS_State.x);
	yDiff = (TS_State.y > ESP_TS.yc) ? (TS_State.y - ESP_TS.yc) : (ESP_TS.yc - TS_State.y);

	/*装载触摸数据-------------------------------------------------------*/
	if ((TS_State.Pressed != ESP_TS.TouchDetected) || (xDiff > 30) || (yDiff > 30)) {
		TS_State.Pressed = ESP_TS.TouchDetected;//按下与没有按下
		if (ESP_TS.TouchDetected) {
			TS_State.x = ESP_TS.xc;
			TS_State.y = ESP_TS.yc;
			GUI_TOUCH_StoreStateEx(&TS_State);
		}
		else {
			GUI_TOUCH_StoreStateEx(&TS_State);
			TS_State.x = 0;
			TS_State.y = 0;
		}

	}
}

