#include "DebugSupport.h"
#include <stdlib.h>

#define ARRAY_LENGTH 40
/*正弦波-------------------------------------------------------*/
u8_t static SineWave[ARRAY_LENGTH] = {
	0x80, 0x94, 0xa7, 0xb9, 0xca, 0xd9, 0xe7, 0xf1, 0xf8, 0xfd,
	0xff, 0xfd, 0xf8, 0xf1, 0xe7, 0xd9, 0xca, 0xb9, 0xa7, 0x94,
	0x80, 0x6c, 0x55, 0x46, 0x35, 0x25, 0x18, 0x0e, 0x06, 0x02,
	0x00, 0x02, 0x06, 0x0e, 0x18, 0x25, 0x35, 0x46, 0x55, 0x6c,
};
/*三角波-------------------------------------------------------*/
u8_t static TriangleWave[ARRAY_LENGTH] = {
	0x80, 0x8c, 0x99, 0xa6, 0xb3, 0xc0, 0xcc, 0xd9, 0xe6, 0xf2,
	0xff, 0xf2, 0xe6, 0xd9, 0xcc, 0xc0, 0xb3, 0xa6, 0x99, 0x8c,
	0x80, 0x73, 0x66, 0x5a, 0x4d, 0x40, 0x33, 0x26, 0x1a, 0x0d,
	0x00, 0x0d, 0x1a, 0x26, 0x33, 0x40, 0x4d, 0x5a, 0x66, 0x73,
};
/*方波-------------------------------------------------------*/
u8_t static SquareWave[ARRAY_LENGTH] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*波形发生-----------------------------------------------------*/
void WaveCreate (u8_t eWaveType, WAVE_TYPE array[],
                 int length, int max,
                 int min, int Cycle) {
	static double factor = 0;//放缩因子
	static int coincide = 0;//周期对应的重叠数
	static int i = 0, n = 0, m = 0;//索引
	static u8_t* p;//波形数组指针
	static ARRAY_TYPE WaveArray[ARRAY_LENGTH] = {0};//临时波形数组
	/*-------------------------------------------------------*/
	factor = (double)(max - min) / 256;//放缩因子
	coincide = (length / ARRAY_LENGTH) / Cycle + 1;//周期对应的重叠数
	/*-------------------------------------------------------*/
	switch (eWaveType) {
		case eSineWave:
			p = SineWave;
			break;
		case eTriangleWave:
			p = TriangleWave;
			break;
		case eSquareWave:
			p = SquareWave;
			break;
		default:
			break;
	}
	for (i = 0; i < ARRAY_LENGTH; i++)
		WaveArray[i] = (int)(factor * (p[i]) + min);//放缩后的波形数组
	m = (rand()) % ARRAY_LENGTH;//随机起始
	for (i = 0; i < length; i += coincide)
		for (n = 0; n < coincide; n++) {
			if (m >= ARRAY_LENGTH)
				m = 0;
			array[i + n] = WaveArray[m++];
		}
}

/*-------------------------------------------------------*/
void Mytouch_MainTask (void) {
	GUI_PID_STATE TouchState;
	int xPhys;
	int yPhys;
	GUI_Init();
	GUI_SetFont(&GUI_Font20_ASCII);
	GUI_CURSOR_Show();
	GUI_CURSOR_Select(&GUI_CursorCrossL);
	GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);
	GUI_Clear();
	GUI_DispString("Measurement of\nA/D converter values");
	while (1) {
		GUI_TOUCH_GetState(&TouchState); // Get the touch position in pixel
		xPhys = GUI_TOUCH_X_MeasureX(); // Get the A/D mesurement result in x
		yPhys = GUI_TOUCH_X_MeasureY(); // Get the A/D mesurement result in y
		GUI_SetColor(GUI_BLUE);
		GUI_DispStringAt("Analog input:\n", 0, 40);
		GUI_GotoY(GUI_GetDispPosY() + 2);
		GUI_DispString("x:");
		GUI_DispDec(xPhys, 4);
		GUI_DispString(", y:");
		GUI_DispDec(yPhys, 4);
		GUI_SetColor(GUI_RED);
		GUI_GotoY(GUI_GetDispPosY() + 4);
		GUI_DispString("\nPosition:\n");
		GUI_GotoY(GUI_GetDispPosY() + 2);
		GUI_DispString("x:");
		GUI_DispDec(TouchState.x, 4);
		GUI_DispString(", y:");
		GUI_DispDec(TouchState.y, 4);
		delay_ms(50);
	};
}

