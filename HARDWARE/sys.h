#pragma once
#include "main.h"

#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long

//Î»´ø²Ù×÷,ÊµÏÖ51ÀàËÆµÄGPIO¿ØÖÆ¹¦ÄÜ
//¾ßÌåÊµÏÖË¼Ïë,²Î¿¼<<CM3È¨ÍþÖ¸ÄÏ>>µÚÎåÕÂ(87Ò³~92Ò³).M4Í¬M3ÀàËÆ,Ö»ÊÇ¼Ä´æÆ÷µØÖ·±äÁË.
//IO¿Ú²Ù×÷ºê¶¨Òå
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
//IO¿ÚµØÖ·Ó³Éä
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010

//IO¿Ú²Ù×÷,Ö»¶Ôµ¥Ò»µÄIO¿Ú!
//È·±£nµÄÖµÐ¡ÓÚ16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //Êä³ö
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //ÊäÈë

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //Êä³ö
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //ÊäÈë

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //Êä³ö
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //ÊäÈë

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //Êä³ö
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //ÊäÈë

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //Êä³ö
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //ÊäÈë

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //Êä³ö
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //ÊäÈë

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //Êä³ö
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //ÊäÈë

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //Êä³ö
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //ÊäÈë

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //Êä³ö
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //ÊäÈë
//////////////////////////////////////////////////////////////////////////////////
//Ex_NVIC_Config×¨ÓÃ¶¨Òå
#define GPIO_A 				0
#define GPIO_B 				1
#define GPIO_C				2
#define GPIO_D 				3
#define GPIO_E 				4
#define GPIO_F 				5
#define GPIO_G 				6
#define GPIO_H 				7
#define GPIO_I 				8

#define FTIR   				1  		//ÏÂ½µÑØ´¥·¢
#define RTIR   				2  		//ÉÏÉýÑØ´¥·¢

//GPIOÉèÖÃ×¨ÓÃºê¶¨Òå
#define GPIO_MODE_IN    	0		//ÆÕÍ¨ÊäÈëÄ£Ê½
#define GPIO_MODE_OUT		1		//ÆÕÍ¨Êä³öÄ£Ê½
#define GPIO_MODE_AF		2		//AF¹¦ÄÜÄ£Ê½
#define GPIO_MODE_AIN		3		//Ä£ÄâÊäÈëÄ£Ê½

#define GPIO_SPEED_2M		0		//GPIOËÙ¶È2Mhz
#define GPIO_SPEED_25M		1		//GPIOËÙ¶È25Mhz
#define GPIO_SPEED_50M		2		//GPIOËÙ¶È50Mhz
#define GPIO_SPEED_100M		3		//GPIOËÙ¶È100Mhz

#define GPIO_PUPD_NONE		0		//²»´øÉÏÏÂÀ­
#define GPIO_PUPD_PU		1		//ÉÏÀ­
#define GPIO_PUPD_PD		2		//ÏÂÀ­
#define GPIO_PUPD_RES		3		//±£Áô

#define GPIO_OTYPE_PP		0		//ÍÆÍìÊä³ö
#define GPIO_OTYPE_OD		1		//¿ªÂ©Êä³ö

//GPIOÒý½Å±àºÅ¶¨Òå
#define PIN0				0x0001
#define PIN1				0x0002
#define PIN2				0x0004
#define PIN3				0x0008
#define PIN4				0x0010
#define PIN5				0x0020
#define PIN6				0x0040
#define PIN7				0x0080
#define PIN8				0x0100
#define PIN9				0x0200
#define PIN10				0x0400
#define PIN11				0x0800
#define PIN12				0x1000
#define PIN13				0x2000
#define PIN14				0x4000
#define PIN15				0x8000

void delay_ms (int count);
void delay_us (int count);
void delay_25ns (int ns);
/*  ---------------------------------------------------------*/
u8 Sys_Clock_Set (u32 plln, u32 pllm, u32 pllp, u32 pllq);		//ÏµÍ³Ê±ÖÓÉèÖÃ
void Stm32_Clock_Init (u32 plln, u32 pllm, u32 pllp, u32 pllq); //Ê±ÖÓ³õÊ¼»¯
void Sys_Soft_Reset (void);      							//ÏµÍ³Èí¸´Î»
void Sys_Standby (void);         							//´ý»úÄ£Ê½
void MY_NVIC_SetVectorTable (u32 NVIC_VectTab, u32 Offset);	//ÉèÖÃÆ«ÒÆµØÖ·
void MY_NVIC_PriorityGroupConfig (u8 NVIC_Group);			//ÉèÖÃNVIC·Ö×é
void MY_NVIC_Init (u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channel, u8 NVIC_Group);//ÉèÖÃÖÐ¶Ï
void Ex_NVIC_Config (u8 GPIOx, u8 BITx, u8 TRIM);				//Íâ²¿ÖÐ¶ÏÅäÖÃº¯Êý(Ö»¶ÔGPIOA~I)
void GPIO_AF_Set (GPIO_TypeDef* GPIOx, u8 BITx, u8 AFx);		//GPIO¸´ÓÃ¹¦ÄÜÉèÖÃ
void GPIO_Set (GPIO_TypeDef* GPIOx, u32 BITx, u32 MODE, u32 OTYPE, u32 OSPEED, u32 PUPD);//GPIOÉèÖÃº¯Êý
//ÒÔÏÂÎª»ã±àº¯Êý
void WFI_SET (void);		//Ö´ÐÐWFIÖ¸Áî
void INTX_DISABLE (void);//¹Ø±ÕËùÓÐÖÐ¶Ï
void INTX_ENABLE (void);	//¿ªÆôËùÓÐÖÐ¶Ï
void MSR_MSP (u32 addr);	//ÉèÖÃ¶ÑÕ»µØÖ·
