#pragma once
#include "main.h"
#define MAX_QUEUE_INIT  0
#define MIN_QUEUE_INIT  255
#define QUEUE_LENGTH    15
/*-------------------------------------------------------*/
uint8_t Trigger_Found (int High, int Low);
uint8_t Trig_Init (WAVE_TYPE Data[]) ;
void OSC_MaxMin_Found (__uIO16* Max_Value, __uIO16* Min_Value, WAVE_TYPE Data[]);
void Quick_Sort (WAVE_TYPE ai[], int low, int high);
int partition (WAVE_TYPE vec[], int low, int high);
u8 OSC_DataDeal (GUI_POINT* WaveArray, int DataLength);

