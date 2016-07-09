#pragma once

#include "main.h"
#include "LCD_MD050SD_Graph.h"
#include "LCD_MD050SD_Touch.h"
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long
/************************************汉字输出相关定义*********************************/
#define  BYT            8
#define  TH            0xa0        //  原网上下载的24×24、32×32字库偏移 0xaf
#define  WH            0xa0        //位码
#define  CODE            6            //汉字字体与图标字体分界
#define  FONTNUM         10           //支持的字体个数
#define  EN_16BUF        16	          //宋体8×16英文缓存
#define  HZ_16BUF        32           //宋体16×16汉字缓存
#define  EN_24BUF        48
#define  HZ_24BUF        72
#define  EN_32BUF         64
#define  HZ_32BUF        128
#define  EN_40BUF        100        //英文取模20×40
#define  HZ_40BUF        200       //WEB等图标字库以80×40取模，用汉字的显示模式

#define  EN_ADD_ST16     15          //宋体8×16ASCII码地址
#define  EN_ADD_ST24     16+1520       //宋体16×24ASCII码地址   1520
#define  EN_ADD_ST32     16+6080       //宋体16×32ASCII码地址   6080
#define  EN_ADD_ST40     16+7907824+1      //宋体20×40ASCII码地址   逐行12160  7907824+1 逐列横向高位在前

#define  HZ_ADD_ST16     16+187792       //宋体16×16汉字地址
#define  HZ_ADD_KT16     16+6337648      //楷体
#define  HZ_ADD_HT16     16+6075952      //黑体暂未定义地址，用雅黑16字库代替
#define  HZ_ADD_YH16     16+6075952      //雅黑
#define  HZ_ADD_XK16     16+7646128      //行楷
#define  HZ_ADD_CY16     16+6075952      //彩云暂未定义地址，用雅黑16字库代替
/**************************************************************************************
*注意32×32的汉字字库，地址是加1、24×24黑体地址＋1、在程序中修正
***************************************************************************************/
#define  HZ_ADD_ST24    16+449488     //宋体24×24汉字库地址   449488
#define  HZ_ADD_KT24    16+1038304    //楷体24×24地址         1038304
#define  HZ_ADD_HT24    16+3262720+1    //黑体24×24地址         3262720
#define  HZ_ADD_YH24    16+2673904    //雅黑24×24地址         2673904
#define  HZ_ADD_XK24    16+4440352    //行楷24×24地址         4440352
#define  HZ_ADD_CY24    16+3851536    //彩云24×24地址         3851536

#define  HZ_ADD_ST32    16+1627120+1    //宋体32×32汉字库地址         1627120
#define  HZ_ADD_KT32    16+6599344+1    //楷体/以行楷32字库代替        6599344
#define  HZ_ADD_HT32    16+5029168+1    //黑体/以雅黑32字库代替        5029168
#define  HZ_ADD_YH32    16+5029168+1    //雅黑                         5029168
#define  HZ_ADD_XK32    16+6599344+1    //行楷                         6599344
#define  HZ_ADD_CY32    16+5029168+1    //彩云/以雅黑32字库代替        5029168

#define  HZ_ADD_ST40    16+0           //宋体40×40汉字库暂未定义地址
#define  HZ_ADD_KT40    16+0            //楷体暂未定义地址
#define  HZ_ADD_HT40    16+0            //黑体暂未定义地址
#define  HZ_ADD_YH40    16+0            //雅黑暂未定义地址
#define  HZ_ADD_XK40    16+0            //行楷暂未定义地址
#define  HZ_ADD_CY40    16+0            //彩云暂未定义地址

#define  WEB_ADD_16     16+23568       //23568
#define  WEB_ADD_24     16+26608       //26608      a到~的显示地址偏移1位，字库问题？？？
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

/////////////////////////////////////用户配置区///////////////////////////////////
//以下2个宏定义，定义屏幕的显示方式及IO速度
#define USE_HORIZONTAL 0	//定义是否使用横屏 		0,不使用.1,使用.
#define LCD_FAST_IO     1 	//定义是否使用快速IO	0,不实用.1,使用
//////////////////////////////////////////////////////////////////////////////////

//TFTLCD部分外要调用的函数
extern u16 POINT_COLOR;//默认红色
extern u16 BACK_COLOR; //背景颜色.默认为白色
//定义LCD的尺寸
// #if USE_HORIZONTAL==1	//使用横屏
#define LCD_W_ 800
#define LCD_H_ 480

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义----------------

//操作频繁，为了提高速度，建议直接寄存器操作
#define	LCD_RS_SET	    GPIOE->ODR|=PIN3    //数据/命令      		PC8
#define	LCD_WR_SET	    GPIOD->ODR|=PIN5    //写数据				PC9
#define	LCD_RD_SET	    GPIOD->ODR|=PIN4   //读数据			 	PC10
#define	LCD_CS_SET      GPIOD->ODR|=PIN7   //片选端口  	    	PC11

#define	LCD_RS_CLR      GPIOE->ODR&=~PIN3   //片选端口  	     	PC8
#define	LCD_WR_CLR	    GPIOD->ODR&=~PIN5   //读数据			 	PC9
#define	LCD_RD_CLR	    GPIOD->ODR&=~PIN4   //写数据			 	PC10
#define	LCD_CS_CLR   	GPIOD->ODR&=~PIN7   //数据/命令         	PC11

#define	LCD_REST_SET	  LCD_REST_GPIO_Port->ODR|=LCD_REST_Pin   //复位     		        PC12
#define	LCD_REST_CLR	  LCD_REST_GPIO_Port->ODR&=~LCD_REST_Pin    //复位      	        PC12
/*  ---------------------------------------------------------*/
//PD0~15,作为数据线
#define LCD_BANK_ADDR                 ((uint32_t)0x60000000)
#define WRITE_READ_ADDR    						 ((uint32_t)0x100000)

#define LCD_CMD LCD_BANK_ADDR
#define LCD_DATA (LCD_BANK_ADDR+WRITE_READ_ADDR)

//读寄存器函数
#define LCD_RD_DATA()  *(__IO uint16_t*) (LCD_DATA)

//写地址
#define LCD_WR_REG(data)   *(__IO uint16_t*) (LCD_BANK_ADDR) = data
//写寄存器函数
#define LCD_WR_DATA(data)  *(__IO uint16_t*) (LCD_BANK_ADDR + WRITE_READ_ADDR) = data

/*  ---------------------------------------------------------*/

//////////////////////////////////////////////////////////////////////

//画笔颜色
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
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

extern u16 BACK_COLOR, POINT_COLOR; //背景色，画笔色

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
//画点
//POINT_COLOR:此点的颜色
inline void ESP_LCD_DrawPoint (u16 x_cur, u16 y_cur) {
	ESP_Address_set(x_cur, y_cur, x_cur, y_cur);//设置光标位置
	LCD_WR_DATA (POINT_COLOR) 

		;
}

/*-------------------------------------------------------*/
inline void LCD_SetCursor (u16 Xpos, u16 Ypos) {
	ESP_Address_set(Xpos, Ypos, Xpos, Ypos);
}

