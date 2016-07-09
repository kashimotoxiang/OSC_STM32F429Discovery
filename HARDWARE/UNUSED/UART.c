#include "UART.h"
/*  ---------------------------------------------------------*/
#define TX_TRANSMIT_SIZE 640
extern ADC_CONV_TYPE ADC_Conv[MEMORYE_DEPTH];
__IO int Offset=0;
char TxBuffer[TX_TRANSMIT_SIZE]={0};
/*  ---------------------------------------------------------*/
#define aRxBuffer   ADC_Conv
#define TXSTARTMESSAGESIZE               (COUNTOF(aTxStartMessage) - 1)
uint8_t aTxStartMessage[] = "\n\r ****UART-Hyperterminal communication based on DMA ****\n\r Enter 10 characters using keyboard :\n\r";
/*  ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
/* USART1 init function */
void MX_USART1_UART_Init(void)
{

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if(HAL_UART_Init(&huart1) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler(); 
	}

}
/*  ---------------------------------------------------------*/
void MX_DMA_UART_Init(void) 
{
	/* UART ---------------------------------------------------------*/
	/* DMA2_Stream7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
}
/*  ---------------------------------------------------------*/
void Open_DMA_UART(volatile uint16_t* sourceTxBuffer,uint16_t Tx_Begin,uint16_t Tx_End)
{
	static uint16_t Tx_i=0,So_i=0,Tx_Size;
	Tx_Size=(Tx_End-Tx_Begin)*2;
	//Tx_Size=Tx_End-Tx_Begin;//制定起始终止
	Tx_Size=Tx_Size<TX_TRANSMIT_SIZE?Tx_Size:TX_TRANSMIT_SIZE;//取源大小和制定大小中的最小值
	for(Tx_i=0,So_i=0;Tx_i<Tx_Size;So_i++)
	{
		TxBuffer[Tx_i++]=sourceTxBuffer[So_i]>>8;
		TxBuffer[Tx_i++]=sourceTxBuffer[So_i]&0xff;
	}
	/*  ---------------------------------------------------------*/
	if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)TxBuffer, Tx_Size)!= HAL_OK)
	{
		/* Transfer error in transmission process */
		Error_Handler();   
	}
}
/*  ---------------------------------------------------------*/
void Check_DMA_UART(void) 
{
	while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
	{
	} 
}
/*  ---------------------------------------------------------*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}
/*  ---------------------------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

}
/***********************************
*函数功能：测试用
*形参说明：
*返回值说明
*创建时间：
***********************************/
void Text(void)
{
	if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)aTxStartMessage, TXSTARTMESSAGESIZE)!= HAL_OK)
	{
		/* Transfer error in transmission process */
		Error_Handler();  
	}
	while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
	{
	} 
}
