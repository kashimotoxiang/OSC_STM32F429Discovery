#pragma once
#include "main.h"

/* touch panel interface define */
#define  CMD_RDX  0xD0	 //触摸IC读坐标积存器
#define  CMD_RDY  0x90
//触摸引脚定义------------------------------------------------------------------------------------------//
#define	T_CLK_High	      Touch_CLK_GPIO_Port->ODR|=Touch_CLK_Pin    //数据/命令      		PA11
#define	T_CS_High	      Touch_CS_GPIO_Port->ODR|=Touch_CS_Pin    //写数据				PA12
#define	T_DIN_High	      Touch_DIN_GPIO_Port->ODR|=Touch_DIN_Pin   //读数据			 	PA13

#define	T_CLK_Low		  Touch_CLK_GPIO_Port->ODR&=~Touch_CLK_Pin     //片选端口  	     	PA11
#define	T_CS_Low	      Touch_CS_GPIO_Port->ODR&=~Touch_CS_Pin     //读数据			 	PA12
#define	T_DIN_Low	      Touch_DIN_GPIO_Port->ODR&=~Touch_DIN_Pin    //写数据			 	PA13

#define  T_IRQ_Dect		  ((Touch_IRQ_GPIO_Port->IDR)&Touch_IRQ_Pin)
#define  T_DOUT_Dect     ((Touch_DOUT_GPIO_Port->IDR)&Touch_DOUT_Pin)

//------------------------------------------------------------------------------------------//

typedef struct Touch_Data_Struct {
	u16 xc;//当前坐标
	u16 yc;
	u16 xl;//上次坐标
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
extern Touch_Data_Struct tp_pixad, ESP_TS; //当前触控坐标的AD值,前触控坐标的像素值
extern u16 vx, vy; //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
extern u16 chx, chy;//默认像素点坐标为0时的AD起始值
void ESP_TS_Init (void);
void Touch_Adjust (void);
void point (void); //绘图函数

u16 ESP_TP_Read_XOY (u8 xy);//SPI （寄存器版本）收发数据
u8 ESP_Convert_Pos (void);
void ESP_Pointer_Update (void);

