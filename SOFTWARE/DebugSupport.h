#pragma once
#define ARRAY_TYPE int
#include "main.h"
/*-------------------------------------------------------*/
typedef unsigned char u8_t;

typedef enum {
	eSineWave = 1,
	eTriangleWave,
	eSquareWave,
} WaveType;//≤®–Œ¿‡–Õ

/*-------------------------------------------------------*/
void WaveCreate (u8_t eWaveType, WAVE_TYPE array[], int length, int max, int min, int Cycle);
void Mytouch_MainTask (void);

