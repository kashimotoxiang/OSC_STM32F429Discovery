#pragma once
#include "main.h"
/*********************************************************************
*
*       extern  
*
**********************************************************************
*/
/*FPGA通信协议控制字-------------------------------------------------------*/
extern uint16_t SPI_InquiryFreq[1];
extern uint16_t SPI_InquiryData[1];
/*********************************************************************
*
*       Declartion
*
**********************************************************************
*/
uint8_t mSPI1_ReadByte (uint8_t TxData);
void mSPI1_WriteByte (uint8_t TxData);
uint16_t mSPI1_ReadWriteByte (uint16_t TxData);
uint8_t mSPI2_ReadWriteByte (uint8_t TxData);
uint8_t mSPI3_ReadWriteByte (uint8_t TxData);

