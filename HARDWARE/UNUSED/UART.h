#ifndef UART_INIT__H
#define UART_INIT__H
/****************************************/
#include "main.h"

void MX_USART1_UART_Init(void);
void MX_DMA_UART_Init(void);
void Open_DMA_UART(volatile uint16_t* sourceTxBuffer,uint16_t Tx_Begin,uint16_t Tx_End) ;
void Check_DMA_UART(void);
void Text(void);

#endif
