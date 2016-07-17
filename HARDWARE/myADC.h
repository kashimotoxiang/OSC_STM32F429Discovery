#pragma once
#include "main.h"
/*-------------------------------------------------------*/
#define	NumOfResult	8
#define	Max_Volt        3.3
#define	Accu_Adc        4095
//---------------------------------------------------------------------//

void Open_ADC_Triple_DMA (void);
void Close_ADC_Triple_DMA (void);
void ADC_DMA_ReSet (void);
void ADC_Timer_ReSet (void);
float Get_ADC1_Average (u8 ch, u8 times);

