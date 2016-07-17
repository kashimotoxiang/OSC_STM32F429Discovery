#include "myADC.h"
uint32_t ADC_SAMPLETIME_xCYCLES = ADC_SAMPLETIME_3CYCLES;

/*  ----------------三重ADC采样-----------------------------------------*/
#if TRIBLE_ADC
/*  ---------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
/*  ---------------------------------------------------------*/
void Open_ADC_Triple_DMA (void) {
	hadc1.Instance->SR &= ~ADC_SR_OVR ;
	hadc2.Instance->SR &= ~ADC_SR_OVR ;
	hadc3.Instance->SR &= ~ADC_SR_OVR ;

	HAL_ADC_Start(&hadc3);
	HAL_ADC_Start(&hadc2);
	HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)g_SamplData.Data, 1500);
}

/*  ---------------------------------------------------------*/
void Close_ADC_Triple_DMA (void) {

}

//---------------------------------------------------------------------//
void ADC_DMA_ReSet (void) {
	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_xCYCLES;

	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	HAL_ADC_ConfigChannel(&hadc2, &sConfig);
	HAL_ADC_ConfigChannel(&hadc3, &sConfig);
	HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)g_SamplData.Data, 1500);

	hadc1.Instance->SR &= ~ADC_SR_OVR ;
	hadc2.Instance->SR &= ~ADC_SR_OVR ;
	hadc3.Instance->SR &= ~ADC_SR_OVR ;
}

//---------------------------------------------------------------------//
void ADC_Timer_ReSet (void) {

}

/*  ---------------------------------------------------------*/
void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef* hadc) {

}
#endif

/*-------------------------------------------------------*/
//ADC1获取n次adc转换求平均的值
float Get_ADC1_Average (u8 ch, u8 times) {
	static u32 temp_val = 0;
	static u8 t;
	for (t = 0; t < times; t++) {
		ADC1->SQR3 &= 0XFFFFFFE0;
		ADC1->SQR3 |= ch;
		ADC1->CR2 |= 1 << 30;
		//while (!(ADC1->SR & 1 << 1));
		temp_val += ADC1->DR;
		delay_ms(1);
	}
	return temp_val / times;
}

