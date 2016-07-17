#pragma once

//*****************************************************************************
//
// #include 
//
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//
// extern  
//
//*****************************************************************************
extern UART_HandleTypeDef huart3;
extern UART_TYPE SPISTARTstr[];
//*****************************************************************************
//
// UART COMMEND FUCTION
//
//*****************************************************************************
#define FPGA_COM_SPISTART mUART3_TransmitIT(SPISTARTstr);

//*****************************************************************************
//
// inline
//
//*****************************************************************************
/*-------------------------------------------------------*
*
*	mUART3_SendAByte
*
*-------------------------------------------------------*/
inline void mUART3_SendAByte (uint8_t TxData) {
	while (!(USART3->SR & (1 << 7)));
	USART3->DR = TxData;
}

/*-------------------------------------------------------*
*
*	mUART3_ReceiveAByte
*
*-------------------------------------------------------*/
inline uint8_t mUART3_ReceiveAByte (void) {
	while (USART3->SR & (1 << 5));//接收到数据 读取数据寄存器不为空 
	return USART3->DR;
}

/*-------------------------------------------------------*
*
*	mUART6_SendAByte
*
*-------------------------------------------------------*/
inline void mUART6_SendAByte (uint8_t TxData) {
	while (!(USART6->SR & (1 << 7)));
	USART6->DR = TxData;
}

/*-------------------------------------------------------*
*
*	mUART6_ReceiveAByte
*
*-------------------------------------------------------*/
inline uint8_t mUART6_ReceiveAByte (void) {
	while (USART6->SR & (1 << 5));//接收到数据 读取数据寄存器不为空 
	return USART6->DR;
}

/*-------------------------------------------------------*
*
*	mUART3_ReceiveIT
*
*-------------------------------------------------------*/
inline void mUART3_ReceiveIT (UART_TYPE RxBuff[]) {
	if (HAL_UART_Receive_IT(&huart3, (uint8_t*)RxBuff, 
		sizeof(RxBuff) / sizeof(*(RxBuff))+1) != HAL_OK)
		Error_Handler();
}

/*-------------------------------------------------------*
*
*	mUART3_TransmitIT
*
*-------------------------------------------------------*/
inline void mUART3_TransmitIT (UART_TYPE TxBuff[]) {
	if (HAL_UART_Transmit_IT(&huart3, (uint8_t*)TxBuff, 
		sizeof(TxBuff) / sizeof(*(TxBuff))+1) != HAL_OK)
		Error_Handler();
}

//*****************************************************************************
//
// declartion
//
//*****************************************************************************
void UART_DMATX (volatile uint16_t* sourceTxBuffer, uint16_t Tx_Begin, uint16_t Tx_End);

