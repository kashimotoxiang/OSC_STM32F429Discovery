#pragma once
#include "main.h"

/* touch panel interface define */
#define  CMD_RDX  0xD0	 //����IC�����������
#define  CMD_RDY  0x90
//�������Ŷ���------------------------------------------------------------------------------------------//
#define	T_CLK_High	      Touch_CLK_GPIO_Port->ODR|=Touch_CLK_Pin    //����/����      		PA11
#define	T_CS_High	      Touch_CS_GPIO_Port->ODR|=Touch_CS_Pin    //д����				PA12
#define	T_DIN_High	      Touch_DIN_GPIO_Port->ODR|=Touch_DIN_Pin   //������			 	PA13

#define	T_CLK_Low		  Touch_CLK_GPIO_Port->ODR&=~Touch_CLK_Pin     //Ƭѡ�˿�  	     	PA11
#define	T_CS_Low	      Touch_CS_GPIO_Port->ODR&=~Touch_CS_Pin     //������			 	PA12
#define	T_DIN_Low	      Touch_DIN_GPIO_Port->ODR&=~Touch_DIN_Pin    //д����			 	PA13

#define  T_IRQ_Dect		  ((Touch_IRQ_GPIO_Port->IDR)&Touch_IRQ_Pin)
#define  T_DOUT_Dect     ((Touch_DOUT_GPIO_Port->IDR)&Touch_DOUT_Pin)

//------------------------------------------------------------------------------------------//

typedef struct Touch_Data_Struct {
	u16 xc;//��ǰ����
	u16 yc;
	u16 xl;//�ϴ�����
	u16 yl;
	u8 TouchDetected;
} Touch_Data_Struct;

struct tp_pixu32_ {
	u32 xc;
	u32 yc;
	u32 xl;
	u32 yl;
};


/*-------------------------------------------------------*/
extern Touch_Data_Struct tp_pixad, ESP_TS; //��ǰ���������ADֵ,ǰ�������������ֵ
extern u16 vx, vy; //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
extern u16 chx, chy;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
void ESP_TS_Init (void);
void Touch_Adjust (void);
void point (void); //��ͼ����

u16 ESP_TP_Read_XOY (u8 xy);//SPI ���Ĵ����汾���շ�����
u8 ESP_Convert_Pos (void);
void ESP_Pointer_Update (void);

