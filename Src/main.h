#pragma once
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "arm_math.h"
#include "stdint.h"
#include "GUI.h"
#include "core_cm4.h"
/* Private define ------------------------------------------------------------*/
#define  TFT_WIDTH           640/2
#define  TFT_BEGINNING   MEMORYE_DEPTH/4
#define  MEMORYE_DEPTH       1500
#define  Zone_Size           0.1
#define  TRIGGER_TOLRATE_EDGE             50
#define  TRIGGER_WIDTH_TOLRATE       10
#define  TRIGGER_CENTRE_TOLRATE    1
#define  SAMPLING_SPEED       4
#define  MAXMIN_ARRAY_SIZE        20
#define  FOUND_SIZE 300

/*-----------------------是否进入调试模式--------------------------------*/

//#define DEBUG__				 //功能查看模式
//#define MEM_DEBUG__			 //内存调试模式
//#define SINGLE_PAGE__		 //使用单页缓存
#define ADCTAKESAMPLE__		 //ADC采样模式填充数组
//#define SPIDATATRANS__		 //SPI数据传输模式

/*-------------------------------------------------------*/

/*----------------------功能裁剪---------------------------------*/

#define TRIBLE_ADC 0
#define QUICK_SORT 0

/*-------------------------------------------------------*/

/*=======================类型定义================================*/
typedef volatile unsigned char __uIO8;
typedef volatile unsigned short int __uIO16;
typedef volatile unsigned int __uIO32;

typedef volatile signed char __IO8;
typedef volatile signed short int __IO16;
typedef volatile signed int __IO32;

typedef unsigned char u8_t;
typedef unsigned short int u16_t;
typedef unsigned short int u32_t;

typedef u8_t u8;
typedef u16_t u16;
typedef u32_t u32;

typedef __IO
unsigned int vu32;
typedef __IO
unsigned short int vu16;
typedef __IO
unsigned char vu8;

typedef __uIO16 WAVE_TYPE;
typedef __uIO16 TFT_DISPLAY_TYPE;
typedef __uIO8 SPI_TYPE;


typedef char UART_TYPE;

/*=========================状态定义==============================*/
//注意初始状态值一定要大于1！！！！！！！！！！！！！！！！

/*-----------------------主状态--------------------------------*/
typedef enum {
	eFSM_MainS_Emp = 0x01,//初始状态大于1
	eFSM_MainS_OSC,
} FSM_MainS_;//主状态

/*-----------------------工作状态--------------------------------*/
typedef enum {
	eFSM_WorkS_Emp = 0x01,//初始状态大于1
	eFSM_WorkS_TriADC,
} FSM_WorkS_;//工作状态

/*-----------------------触发模式--------------------------------*/
typedef enum {
	eTrg_Mod_Begin = 0x01,//初始状态大于1
	eTrg_Mod_Rising = eTrg_Mod_Begin,
	eTrg_Mod_Falling,
	eTrg_Mod_Centr,
	eTrg_Mod_End,
} Trg_Mod_;//触发模式

/*-----------------------采样方式--------------------------------*/
typedef enum {
	eSampl_Mod_DMA = 0x01,//初始状态大于1
	eSampl_Mod_Timer,
	eSampl_Mod_Equal,
} Sampl_Mod_;//采样方式

/*------------------------页数-------------------------------*/
typedef enum {
	ePage_Num_Begin = 0x01,//初始状态大于1
	ePage_Num_1 = ePage_Num_Begin,
	ePage_Num_2,
	ePage_Num_3,
	ePage_Num_4,
	ePage_Num_5,
	ePage_Num_6,
	ePAGE_End = ePage_Num_6,
} Page_Num_;//页数

/*-------------------------分辨率------------------------------*/
typedef enum {
	eReslt_rat_Begin = 0x01,//初始状态大于1
	eReslt_rat_0 = eReslt_rat_Begin,
	eReslt_rat_1,
	eReslt_rat_2,
	eReslt_rat_3,
	eReslt_rat_4,
	eReslt_rat_5,
	eReslt_rat_6,
	eReslt_rat_7,
	eReslt_rat_8,
	eReslt_rat_9,
	eReslt_rat_10,
	eReslt_rat_11,
	eReslt_rat_End = eReslt_rat_11,
} Reslt_rat_;//分辨率

/*---------------------触摸屏按键状态----------------------------------*/
typedef enum {
	//初始状态大于1
	eKey_Empty = 0x01,
	eSampl_rat_ADD,
	eReslt_rat_ADD,
	eReslt_rat_SUB,
	eTRGPosLine_ADD,
	eTRGPosLine_SUB,
	eTRG_Mod_ADD,
	eTRG_Mod_SUB,
} Key_State_;//触摸屏按键状态

/*------------------------u16边界检查-------------------------------*/
typedef enum {
	eu16_BEGIN =0,
	eu16_END = 65535,
} u16_BEGIN_;//

/*------------------------开关-------------------------------*/
typedef enum {
	eClose = 0,
	eOpen = 1,
} Switch_;//

/*------------------------LCD参数设置-------------------------------*/
typedef enum {
	eLCDPar_width = 800, //LCD 宽度
	eLCDPar_height = 480, //LCD 高度
	eLCDPar_dir = 1, //横屏还是竖屏控制：0，竖屏；1，横屏。	
	eLCDPar_wramcmd = 0x0F, //开始写gram指令
	eLCDPar_setxcmd = 0x02, //设置x坐标指令
	eLCDPar_setycmd = 0x03, //设置y坐标指令 
} LCD_Par_;

/*------------------------LCD设备-------------------------------*/
typedef enum {
	eLAYER_BSP = 0,
	eLAYER_ESP = 1,
} LAYER_Par_;

/*------------------------GUI控制字-------------------------------*/
typedef enum {
	eNumpadInput = 0,
} GUI_CONTROL_;//
/*------------------------SPI控制字-------------------------------*/
typedef enum {
	eS_SendFreq=0x0000,
	eS_SendData=0xFFFF,
} SPI_CONTROL_;//
/*=========================结构体定义==============================*/

/* ADC ----------------------------------------------------------*/
typedef struct ADC_Conv_struct {
	WAVE_TYPE Data[MEMORYE_DEPTH];
	uint16_t FFT_Data[MEMORYE_DEPTH / 2];
} ADC_Conv_struct;

/* FMS ---------------------------------------------------------*/
typedef struct FSM_struct//FMS
{
	__uIO8 WorkS;
	__uIO8 Old_WorkS;
	__uIO8 MainS;
	__uIO8 Old_MainS;
} FSM_struct;

/* OSC ---------------------------------------------------------*/
typedef struct OSC_struct//OSC
{
	__uIO8 Trig_Mod;
	__uIO8 Sampl_Mod;
	__uIO8 Res_rat;
	__uIO32 TRG_Pos;
	__uIO32 TRG_Pos_Old;
	__uIO16 i_MaxVal;
	__uIO16 i_MinVal;
	float f_MaxVal;
	float f_MinVal;
	__uIO32 Freq;
	__uIO32 DataBegin;
	__uIO32 DataEnd;
	__uIO32 ClyDotNum;
	u8 MeasureStatu;
} OSC_struct;

/* GUIControl ---------------------------------------------------------*/
typedef struct GUIControl_struct//OSC
{
	__uIO8 InputDialogState;
	__uIO8 DispState;
} GUIControl_struct;

/* LCD---------------------------------------------------------*/
typedef struct Page_struct//LCD
{
	__uIO8 One;//显示页
	__uIO8 Two;//操作页
	__uIO8 Temp;//交换页码用
} Page_struct;

/* Key---------------------------------------------------------*/
typedef struct Key_struct//Key
{
	__uIO8 Cur;
	__uIO8 Last;
} Key_struct;

/* Key---------------------------------------------------------*/
typedef struct Index_struct//Key
{
	__uIO8 Cur;
	__uIO8 Last;
} Index_struct;

/*=======================================================*/
extern ADC_Conv_struct g_SamplData;
extern FSM_struct g_FSM;
extern OSC_struct g_OSCInfo;
extern Page_struct g_DispPage;
extern Key_struct g_Key;
extern GUIControl_struct g_GUIControl;

extern void Error_Handler (void);
/* Includes ------------------------------------------------------------------*/

/*硬件-------------------------------------------------------*/
#include "System.h"
#include "mySPI.h"
#include "myUART.h"
#include "myADC.h"
#include "myKey.h"
#include "LCD_MD050SD.h"
#include "LCD_ILI9341.h"
/*软件-------------------------------------------------------*/
#include "Usual.h"
#include "FSM.h"
#include "OSC_Work.h"
#include "FFT.h"
/*emWin-------------------------------------------------------*/
#include "GUIDefine.h"
#include "stm32f429i_discovery_ts.h"
#include "stm32f429i_discovery_io.h"
#include "stm32f429i_discovery.h"
#include "ts.h"
#include "calibration.h"
/*-------------------------------------------------------*/
#ifdef DEBUG__
#include "DebugSupport.h"
#endif

#ifdef MEM_DEBUG__
#include "GUIUsual.h"
#endif
/*-------------------------------------------------------*/


