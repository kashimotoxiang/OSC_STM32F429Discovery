#include "myKey.h"
extern __uIO32 g_DACVal;
/*-------------------------------------------------------*/
#if 0
void Touch_Key_OSC (void) {
	if (uDiffABS (tp.xc, tp.xl) < 10 && uDiffABS (tp.yc, tp.yl) < 10)
		return;
	tp.xl = tp.xc;
	tp.yl = tp.yc;

	if (tp.xc > Background_Line_W_9 && tp.xc < Background_Line_W_10)//在按键区内
	{
/*  分别判断具体案按键---------------------------------------------------------*/
		if (isBound (tp.yc, Background_Line_H_0, Background_Line_H_1))
			Key.Cur = eSampl_rat_ADD;

		else if (isBound (tp.yc, Background_Line_H_1, Background_Line_H_2))
			Key.Cur = eReslt_rat_ADD;

		else if (isBound (tp.yc, Background_Line_H_2, Background_Line_H_3))
			Key.Cur = eReslt_rat_SUB;

		else if (isBound (tp.yc, Background_Line_H_3, Background_Line_H_4))
			Key.Cur = eTRGPosLine_ADD;

		else if (isBound (tp.yc, Background_Line_H_4, Background_Line_H_5))
			Key.Cur = eTRGPosLine_SUB;

		else if (isBound (tp.yc, Background_Line_H_5, Background_Line_H_6))
			Key.Cur = eTRG_Mod_ADD;
	}
/*  选择按键功能---------------------------------------------------------*/
	switch (Key.Cur) {
		case eSampl_rat_ADD:
			OSC.Sampl_Mod = ~OSC.Sampl_Mod;
			break;
		case eReslt_rat_ADD:
			OSC.Res_rat++;
			break;
		case eReslt_rat_SUB:
			OSC.Res_rat--;
			break;
		case eTRGPosLine_ADD:
			OSC.TRG_Pos += 80;
			break;
		case eTRGPosLine_SUB:
			OSC.TRG_Pos -= 80;
			break;
		case eTRG_Mod_ADD:
			OSC.Trig_Mod++;
			break;
		default:
			break;
	}
/*初始化按键-------------------------------------------------------*/
	Key.Last = Key.Cur;
	Key.Cur = 0;
//---------------------------------------------------------------------//
	if (OSC.Res_rat == eReslt_rat_Begin)
		OSC.Res_rat = eReslt_rat_11;
	else if (OSC.Res_rat == eReslt_rat_End)
		OSC.Res_rat = eReslt_rat_0;
//---------------------------------------------------------------------//
	if (OSC.TRG_Pos > 640)
		OSC.TRG_Pos = 0;
	else if (OSC.TRG_Pos < 0)
		OSC.TRG_Pos = 640;
//---------------------------------------------------------------------//
	if (OSC.Trig_Mod == eTrg_Mod_Begin)
		OSC.Trig_Mod = eTrg_Mod_End - 1;
	else if (OSC.Trig_Mod == eTrg_Mod_End)
		OSC.Trig_Mod = eTrg_Mod_Begin + 1;
//---------------------------------------------------------------------//
	return;
}
#endif
/*-------------------------------------------------------*/
void Touch_Key_DAC (void) {
	if (uDiffABS (ESP_TS.xc, ESP_TS.xl) < 10 && uDiffABS (ESP_TS.yc, ESP_TS.yl) < 10)
		return;
	ESP_TS.xl = ESP_TS.xc;
	ESP_TS.yl = ESP_TS.yc;

	if (ESP_TS.xc > 400)
		g_DACVal += 256;
	else
		g_DACVal -= 256;
	g_DACVal = Safe_Return((g_DACVal), 0, 4095) ;
}

