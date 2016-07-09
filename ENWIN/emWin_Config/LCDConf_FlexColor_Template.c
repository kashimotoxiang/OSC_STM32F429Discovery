#include "GUI.h"
#include "GUIDRV_Template_ESP.h"
#include "GUIDRV_Template_BSP.h"
#include "GUIDRV_FlexColor.h"
#include <main.h>

//�봥�����йض��壬����ʵ�������д
#define TOUCH_AD_TOP		3388  	//���´������Ķ�����д�� Y ��ģ������ֵ��
#define TOUCH_AD_BOTTOM		6078 	//���´������ĵײ���д�� Y ��ģ������ֵ��
#define TOUCH_AD_LEFT 		247		//���´���������࣬д�� X ��ģ������ֵ��
#define TOUCH_AD_RIGHT		4421	//���´��������Ҳ࣬д�� X ��ģ������ֵ��


//��Ļ��С
#define XSIZE_PHYS  800 //X��
#define YSIZE_PHYS  480 //Y��
#define VXSIZE_PHYS	800 
#define VYSIZE_PHYS 480


//���ü��
#ifndef VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef GUICC_565
  #error Color conversion not defined!
#endif
#ifndef GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif


//���ó���,���ڴ�����ʾ��������,������ɫת���������ʾ�ߴ�
void LCD_X_Config (void) {
			GUI_MULTIBUF_ConfigEx(eLAYER_BSP, 1);
			GUI_MULTIBUF_ConfigEx(eLAYER_ESP, 1);
	/*BSP-------------------------------------------------------*/
	GUI_DEVICE_CreateAndLink(&BSP_GUIDRV_Template_API, GUICC_M565, 0, eLAYER_BSP); //������ʾ��������
	LCD_SetSizeEx(eLAYER_BSP, 240, 320);
	LCD_SetVSizeEx(eLAYER_BSP, 240, 320);

	GUI_TOUCH_Calibrate(GUI_COORD_X, 0, 240, 0, 239);
	GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, 320, 0, 319);
	/*ESP-------------------------------------------------------*/
	GUI_DEVICE_CreateAndLink(&ESP_GUIDRV_Template_API, GUICC_M565, 0, eLAYER_ESP); //������ʾ��������
	LCD_SetSizeEx(eLAYER_ESP, XSIZE_PHYS, YSIZE_PHYS);
	LCD_SetVSizeEx(eLAYER_ESP, XSIZE_PHYS, YSIZE_PHYS);

	GUI_TOUCH_Calibrate(GUI_COORD_X, 0, 480, 0, 479);
	GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, 800, 0, 799);


}

//��ʾ�������Ļص�����
int LCD_X_DisplayDriver (unsigned LayerIndex, unsigned Cmd, void* pData) {
	int r;
	(void) LayerIndex;
	(void) pData;

	switch (Cmd) {
		case LCD_X_INITCONTROLLER: {
			//����ʼ����ʱ�򱻵���,��Ҫ��������ʾ������,�����ʾ���������ⲿ��ʼ������Ҫ�û���ʼ��

			//	TFTLCD_Init(); //��ʼ��LCD �Ѿ��ڿ�ʼ��ʼ����,���Դ˴�����Ҫ��ʼ����
			return 0;
		}
		default:
			r = -1;
	}
	return r;
}

