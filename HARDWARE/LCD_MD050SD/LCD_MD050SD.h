#pragma once

#include "main.h"
#include "LCD_MD050SD_Graph.h"
#include "LCD_MD050SD_Touch.h"
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long
/************************************���������ض���*********************************/
#define  BYT            8
#define  TH            0xa0        //  ԭ�������ص�24��24��32��32�ֿ�ƫ�� 0xaf
#define  WH            0xa0        //λ��
#define  CODE            6            //����������ͼ������ֽ�
#define  FONTNUM         10           //֧�ֵ��������
#define  EN_16BUF        16	          //����8��16Ӣ�Ļ���
#define  HZ_16BUF        32           //����16��16���ֻ���
#define  EN_24BUF        48
#define  HZ_24BUF        72
#define  EN_32BUF         64
#define  HZ_32BUF        128
#define  EN_40BUF        100        //Ӣ��ȡģ20��40
#define  HZ_40BUF        200       //WEB��ͼ���ֿ���80��40ȡģ���ú��ֵ���ʾģʽ

#define  EN_ADD_ST16     15          //����8��16ASCII���ַ
#define  EN_ADD_ST24     16+1520       //����16��24ASCII���ַ   1520
#define  EN_ADD_ST32     16+6080       //����16��32ASCII���ַ   6080
#define  EN_ADD_ST40     16+7907824+1      //����20��40ASCII���ַ   ����12160  7907824+1 ���к����λ��ǰ

#define  HZ_ADD_ST16     16+187792       //����16��16���ֵ�ַ
#define  HZ_ADD_KT16     16+6337648      //����
#define  HZ_ADD_HT16     16+6075952      //������δ�����ַ�����ź�16�ֿ����
#define  HZ_ADD_YH16     16+6075952      //�ź�
#define  HZ_ADD_XK16     16+7646128      //�п�
#define  HZ_ADD_CY16     16+6075952      //������δ�����ַ�����ź�16�ֿ����
/**************************************************************************************
*ע��32��32�ĺ����ֿ⣬��ַ�Ǽ�1��24��24�����ַ��1���ڳ���������
***************************************************************************************/
#define  HZ_ADD_ST24    16+449488     //����24��24���ֿ��ַ   449488
#define  HZ_ADD_KT24    16+1038304    //����24��24��ַ         1038304
#define  HZ_ADD_HT24    16+3262720+1    //����24��24��ַ         3262720
#define  HZ_ADD_YH24    16+2673904    //�ź�24��24��ַ         2673904
#define  HZ_ADD_XK24    16+4440352    //�п�24��24��ַ         4440352
#define  HZ_ADD_CY24    16+3851536    //����24��24��ַ         3851536

#define  HZ_ADD_ST32    16+1627120+1    //����32��32���ֿ��ַ         1627120
#define  HZ_ADD_KT32    16+6599344+1    //����/���п�32�ֿ����        6599344
#define  HZ_ADD_HT32    16+5029168+1    //����/���ź�32�ֿ����        5029168
#define  HZ_ADD_YH32    16+5029168+1    //�ź�                         5029168
#define  HZ_ADD_XK32    16+6599344+1    //�п�                         6599344
#define  HZ_ADD_CY32    16+5029168+1    //����/���ź�32�ֿ����        5029168

#define  HZ_ADD_ST40    16+0           //����40��40���ֿ���δ�����ַ
#define  HZ_ADD_KT40    16+0            //������δ�����ַ
#define  HZ_ADD_HT40    16+0            //������δ�����ַ
#define  HZ_ADD_YH40    16+0            //�ź���δ�����ַ
#define  HZ_ADD_XK40    16+0            //�п���δ�����ַ
#define  HZ_ADD_CY40    16+0            //������δ�����ַ

#define  WEB_ADD_16     16+23568       //23568
#define  WEB_ADD_24     16+26608       //26608      a��~����ʾ��ַƫ��1λ���ֿ����⣿����
#define  WEB_ADD_32     16+33456       //33456
#define  WEB_ADD_40     16+45616       //45616

#define  WEB_DINGS_16     16+64624     //64624
#define  WEB_DINGS_24     16+67664     //67664
#define  WEB_DINGS_32     16+74512     //74512
#define  WEB_DINGS_40     16+86672     //86672

#define  WING_DINGS_16     16+105680    //105680
#define  WING_DINGS_24     16+108720    //108720
#define  WING_DINGS_32     16+115568    //115568
#define  WING_DINGS_40     16+127728    //127728

#define  WING_DINGS2_16     16+146736    //146736
#define  WING_DINGS2_24     16+149776    //149776
#define  WING_DINGS2_32     16+156624    //156624
#define  WING_DINGS2_40     16+168784    //168784
/******************************************************************************************/

/////////////////////////////////////�û�������///////////////////////////////////
//����2���궨�壬������Ļ����ʾ��ʽ��IO�ٶ�
#define USE_HORIZONTAL 0	//�����Ƿ�ʹ�ú��� 		0,��ʹ��.1,ʹ��.
#define LCD_FAST_IO     1 	//�����Ƿ�ʹ�ÿ���IO	0,��ʵ��.1,ʹ��
//////////////////////////////////////////////////////////////////////////////////

//TFTLCD������Ҫ���õĺ���
extern u16 POINT_COLOR;//Ĭ�Ϻ�ɫ
extern u16 BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
//����LCD�ĳߴ�
// #if USE_HORIZONTAL==1	//ʹ�ú���
#define LCD_W_ 800
#define LCD_H_ 480

////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���----------------

//����Ƶ����Ϊ������ٶȣ�����ֱ�ӼĴ�������
#define	LCD_RS_SET	    GPIOE->ODR|=PIN3    //����/����      		PC8
#define	LCD_WR_SET	    GPIOD->ODR|=PIN5    //д����				PC9
#define	LCD_RD_SET	    GPIOD->ODR|=PIN4   //������			 	PC10
#define	LCD_CS_SET      GPIOD->ODR|=PIN7   //Ƭѡ�˿�  	    	PC11

#define	LCD_RS_CLR      GPIOE->ODR&=~PIN3   //Ƭѡ�˿�  	     	PC8
#define	LCD_WR_CLR	    GPIOD->ODR&=~PIN5   //������			 	PC9
#define	LCD_RD_CLR	    GPIOD->ODR&=~PIN4   //д����			 	PC10
#define	LCD_CS_CLR   	GPIOD->ODR&=~PIN7   //����/����         	PC11

#define	LCD_REST_SET	  LCD_REST_GPIO_Port->ODR|=LCD_REST_Pin   //��λ     		        PC12
#define	LCD_REST_CLR	  LCD_REST_GPIO_Port->ODR&=~LCD_REST_Pin    //��λ      	        PC12
/*  ---------------------------------------------------------*/
//PD0~15,��Ϊ������
#define LCD_BANK_ADDR                 ((uint32_t)0x60000000)
#define WRITE_READ_ADDR    						 ((uint32_t)0x100000)

#define LCD_CMD LCD_BANK_ADDR
#define LCD_DATA (LCD_BANK_ADDR+WRITE_READ_ADDR)

//���Ĵ�������
#define LCD_RD_DATA()  *(__IO uint16_t*) (LCD_DATA)

//д��ַ
#define LCD_WR_REG(data)   *(__IO uint16_t*) (LCD_BANK_ADDR) = data
//д�Ĵ�������
#define LCD_WR_DATA(data)  *(__IO uint16_t*) (LCD_BANK_ADDR + WRITE_READ_ADDR) = data

/*  ---------------------------------------------------------*/

//////////////////////////////////////////////////////////////////////

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ

#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

extern u16 BACK_COLOR, POINT_COLOR; //����ɫ������ɫ

void ESP_LCD_WriteReg (u16 LCD_Reg, u16 LCD_RegValue);
void ESP_LCD_DisplayOn (void);
void ESP_LCD_DisplayOff (void);
void ESP_Address_set (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void ESP_LCD_Init (void);
void ESP_LCD_Clear (u16 Color);
void showhanzi (unsigned int x_cur, unsigned int y_cur, unsigned char index);
//void LCD_Show_Char (u16 x_cur, u16 y_cur, u8 num, u8 mode);
void ESP_LCD_Show_Num (u16 x_cur, u16 y_cur, u32 num, u8 len);
void ESP_LCD_ShowString (u16 x_cur, u16 y_cur, char* p);
void ESP_WritePage (unsigned char index);
void ESP_ShowPage (unsigned char index);
u16 ESP_LCD_ReadPoint (u16 x, u16 y);
void ESP_SwapPage (Page_struct* a);
void ESP_LCD_DrawPoint (u16 x_cur, u16 y_cur);
void LCD_SetCursor (u16 Xpos, u16 Ypos);

/*-------------------------------------------------------*/
//����
//POINT_COLOR:�˵����ɫ
inline void ESP_LCD_DrawPoint (u16 x_cur, u16 y_cur) {
	ESP_Address_set(x_cur, y_cur, x_cur, y_cur);//���ù��λ��
	LCD_WR_DATA (POINT_COLOR) 

		;
}

/*-------------------------------------------------------*/
inline void LCD_SetCursor (u16 Xpos, u16 Ypos) {
	ESP_Address_set(Xpos, Ypos, Xpos, Ypos);
}

