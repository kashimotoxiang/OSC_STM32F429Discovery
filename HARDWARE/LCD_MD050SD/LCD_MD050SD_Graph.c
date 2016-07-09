#include "LCD_MD050SD_Graph.h"

//������ͼ����

//��һ�����
//POINT_COLOR:�˵����ɫ
void ESP_LCD_DrawPoint_big (u16 x_cur, u16 y_cur) {
	ESP_LCD_Fill(x_cur - 1, y_cur - 1, x_cur + 1, y_cur + 1, POINT_COLOR);
}

//��ָ�����������ָ����ɫ
//�����С:
//  (xend-xsta)*(yend-ysta)
void ESP_LCD_Fill (u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color) {
	u16 i, j;
	ESP_Address_set(xsta, ysta, xend, yend); //���ù��λ��
	for (i = ysta; i <= yend; i++) {
		for (j = xsta; j <= xend; j++)
		LCD_WR_DATA(color);
		//���ù��λ��
	}
}

//����
//x1,y1:�������
//x2,y2:�յ�����
void ESP_LCD_DrawLine (u16 x1, u16 y1, u16 x2, u16 y2) {
	u16 t;
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
		ESP_LCD_DrawPoint(uRow, uCol);//����
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
void ESP_LCD_DrawRectangle (u16 x1, u16 y1, u16 x2, u16 y2) {
	ESP_LCD_DrawLine(x1, y1, x2, y1);
	ESP_LCD_DrawLine(x1, y1, x1, y2);
	ESP_LCD_DrawLine(x1, y2, x2, y2);
	ESP_LCD_DrawLine(x2, y1, x2, y2);
}

//��ָ��λ�û�һ��ָ����С��Բ
//(x_cur,y_cur):���ĵ�
//r    :�뾶
void ESP_Draw_Circle (u16 x0, u16 y0, u8 r) {
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1); //�ж��¸���λ�õı�־
	while (a <= b) {
		ESP_LCD_DrawPoint(x0 - b, y0 - a); //3
		ESP_LCD_DrawPoint(x0 + b, y0 - a); //0
		ESP_LCD_DrawPoint(x0 - a, y0 + b); //1
		ESP_LCD_DrawPoint(x0 - b, y0 - a); //7
		ESP_LCD_DrawPoint(x0 - a, y0 - b); //2
		ESP_LCD_DrawPoint(x0 + b, y0 + a); //4
		ESP_LCD_DrawPoint(x0 + a, y0 - b); //5
		ESP_LCD_DrawPoint(x0 + a, y0 + b); //6
		ESP_LCD_DrawPoint(x0 - b, y0 + a);
		a++;
		//ʹ��Bresenham�㷨��Բ
		if (di < 0)di += 4 * a + 6;
		else {
			di += 10 + 4 * (a - b);
			b--;
		}
		ESP_LCD_DrawPoint(x0 + a, y0 + b);
	}
}

//��������
//x0,y0:��� x1,y1:�յ�
//color:��ɫ
void ESP_LCD_Draw_Line (u16 x0, u16 y0, u16 x1, u16 y1, u16 color) {
	u16 tempcolor = POINT_COLOR;
	POINT_COLOR = color;
	ESP_LCD_DrawLine(x0, y0, x1, y1);
	POINT_COLOR = tempcolor;
}

//����ֱ��
//x0,y0:����
//len:�߳���
//color:��ɫ
void ESP_LCD_Draw_Vline (u16 x0, u16 y0, u16 len, u16 color) {
	if (len == 0)return;
	ESP_LCD_Fill(x0, y0, x0, y0 + len - 1, color);
}

//��ˮƽ��
//x0,y0:����
//len:�߳���
//color:��ɫ
void ESP_LCD_Draw_Hline (u16 x0, u16 y0, u16 len, u16 color) {
	if (len == 0)return;
	ESP_LCD_Fill(x0, y0, x0 + len - 1, y0, color);
}

//��ʵ��Բ
//x0,y0:����
//r�뾶
//color:��ɫ
void ESP_LCD_Fill_Circle (u16 x0, u16 y0, u16 r, u16 color) {
	u32 i;
	u32 imax = ((u32)r * 707) / 1000 + 1;
	u32 sqmax = (u32)r * (u32)r + (u32)r / 2;
	u32 x_cur = r;
	ESP_LCD_Draw_Hline(x0 - r, y0, 2 * r, color);
	for (i = 1; i <= imax; i++) {
		if ((i * i + x_cur * x_cur) > sqmax) {
			// draw lines from outside
			if (x_cur > imax) {
				ESP_LCD_Draw_Hline(x0 - i + 1, y0 + x_cur, 2 * (i - 1), color);
				ESP_LCD_Draw_Hline(x0 - i + 1, y0 - x_cur, 2 * (i - 1), color);
			}
			x_cur--;
		}
		// draw lines from inside (center)
		ESP_LCD_Draw_Hline(x0 - x_cur, y0 + i, 2 * x_cur, color);
		ESP_LCD_Draw_Hline(x0 - x_cur, y0 - i, 2 * x_cur, color);
	}
}

//����Բ
//x0,y0:����
//rx:x_cur����뾶
//ry:y_cur����뾶
//color:��Բ����ɫ
void ESP_LCD_Draw_Ellipse (u16 x0, u16 y0, u16 rx, u16 ry, u16 color) {
	u32 OutConst, Sum, SumY;
	u16 x_cur, y_cur;
	u16 xOld;
	u32 _rx = rx;
	u32 _ry = ry;
	if (rx > x0 || ry > y0)return;//�Ƿ�.
	OutConst = _rx * _rx * _ry * _ry + (_rx * _rx * _ry >> 1); // Constant as explaint above
	// To compensate for rounding
	xOld = x_cur = rx;
	for (y_cur = 0; y_cur <= ry; y_cur++) {
		if (y_cur == ry)x_cur = 0;
		else {
			SumY = ((u32)(rx * rx)) * ((u32)(y_cur * y_cur)); // Does not change in loop
			while (Sum = SumY + ((u32)(ry * ry)) * ((u32)(x_cur * x_cur)) , (x_cur > 0) && (Sum > OutConst)) x_cur--;
		}
		// Since we draw lines, we can not draw on the first iteration
		if (y_cur) {
			ESP_LCD_Draw_Line(x0 - xOld, y0 - y_cur + 1, x0 - x_cur, y0 - y_cur, color);
			ESP_LCD_Draw_Line(x0 - xOld, y0 + y_cur - 1, x0 - x_cur, y0 + y_cur, color);
			ESP_LCD_Draw_Line(x0 + xOld, y0 - y_cur + 1, x0 + x_cur, y0 - y_cur, color);
			ESP_LCD_Draw_Line(x0 + xOld, y0 + y_cur - 1, x0 + x_cur, y0 + y_cur, color);
		}
		xOld = x_cur;
	}
}

//�����Բ
//x0,y0:����
//rx:x_cur����뾶
//ry:y_cur����뾶
//color:��Բ����ɫ
void ESP_LCD_Fill_Ellipse (u16 x0, u16 y0, u16 rx, u16 ry, u16 color) {
	u32 OutConst, Sum, SumY;
	u16 x_cur, y_cur;
	u32 _rx = rx;
	u32 _ry = ry;
	OutConst = _rx * _rx * _ry * _ry + (_rx * _rx * _ry >> 1); // Constant as explaint above
	// To compensate for rounding
	x_cur = rx;
	for (y_cur = 0; y_cur <= ry; y_cur++) {
		SumY = ((u32)(rx * rx)) * ((u32)(y_cur * y_cur)); // Does not change in loop
		while (Sum = SumY + ((u32)(ry * ry)) * ((u32)(x_cur * x_cur)) , (x_cur > 0) && (Sum > OutConst))x_cur--;
		ESP_LCD_Draw_Hline(x0 - x_cur, y0 + y_cur, 2 * x_cur, color);
		if (y_cur)ESP_LCD_Draw_Hline(x0 - x_cur, y0 - y_cur, 2 * x_cur, color);
	}
}

