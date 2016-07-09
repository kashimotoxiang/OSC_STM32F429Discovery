#ifndef  __ADC_WHEEL__H
#define __ADC_WHEEL__H

#include "main.h"

#define	NumOfResult	8
#define	Max_Volt        3.3
#define	Accu_Adc        4095
//---------------------------------------------------------------------//
extern volatile uint32_t   ADC_Result;
extern volatile uint8_t    FSM_Condition;
extern volatile uint8_t    Sample_Speed;
//---------------------------------------------------------------------//
void ADC_DMA_Init(void);
void ADC_DMA_Disable(void);
void Open_ADC_Triple_DMA(void);
void Close_ADC_Triple_DMA(void);
void ADC_DMA_ReSet(void);
void ADC_Timer_ReSet(void);
void Deal_Graph_Level(uint16_t __Graph_Level);
void MX_DMA_ADC_Init(void);
#endif
