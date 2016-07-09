#include "ADC_Init.h"
uint32_t ADC_SAMPLETIME_xCYCLES = ADC_SAMPLETIME_3CYCLES;
/*  ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;

DMA_HandleTypeDef hdma_adc1;
/*  ---------------------------------------------------------*/
static void MX_ADC1_Init (void);
static void MX_ADC2_Init (void);
static void MX_ADC3_Init (void);

/*  ---------------------------------------------------------*/
/***********************************
*函数功能：功能配置
*形参说明：
*返回值说明
*创建时间：
***********************************/
void ADC_DMA_Init (void) {
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_ADC3_Init();
}

/*  ---------------------------------------------------------*/
void Open_ADC_Triple_DMA (void) {
	hadc1.Instance->SR &= ~ADC_SR_OVR ;
	hadc2.Instance->SR &= ~ADC_SR_OVR ;
	hadc3.Instance->SR &= ~ADC_SR_OVR ;

	HAL_ADC_Start(&hadc3);
	HAL_ADC_Start(&hadc2);
	HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)ADC_Conv, 1500);
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
	HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)ADC_Conv, 1500);

	hadc1.Instance->SR &= ~ADC_SR_OVR ;
	hadc2.Instance->SR &= ~ADC_SR_OVR ;
	hadc3.Instance->SR &= ~ADC_SR_OVR ;
}

//---------------------------------------------------------------------//
void ADC_Timer_ReSet (void) {

}

//---------------------------------------------------------------------//
void Deal_Graph_Level (uint16_t __Graph_Level) {
	switch (__Graph_Level) {
		case GRAPH_LEVEL_0: ADC_SAMPLETIME_xCYCLES = ADC_SAMPLETIME_3CYCLES ;
			break;
		case GRAPH_LEVEL_1: ADC_SAMPLETIME_xCYCLES = ADC_SAMPLETIME_15CYCLES ;
			break;
		case GRAPH_LEVEL_2: ADC_SAMPLETIME_xCYCLES = ADC_SAMPLETIME_28CYCLES ;
			break;
		case GRAPH_LEVEL_3: ADC_SAMPLETIME_xCYCLES = ADC_SAMPLETIME_56CYCLES ;
			break;
		case GRAPH_LEVEL_4: ADC_SAMPLETIME_xCYCLES = ADC_SAMPLETIME_84CYCLES ;
			break;
		case GRAPH_LEVEL_5: ADC_SAMPLETIME_xCYCLES = ADC_SAMPLETIME_112CYCLES;
			break;
		case GRAPH_LEVEL_6: ADC_SAMPLETIME_xCYCLES = ADC_SAMPLETIME_144CYCLES;
			break;
		case GRAPH_LEVEL_7: ADC_SAMPLETIME_xCYCLES = ADC_SAMPLETIME_480CYCLES;
			break;
		default:
			break;
	}
}

/*  ---------------------------------------------------------*/
/***********************************
*函数功能：系统配置
*形参说明：
*返回值说明
*创建时间：
***********************************/
/* ADC1 init function */
static void MX_ADC1_Init (void) {

	ADC_MultiModeTypeDef multimode;
	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
	*/
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	HAL_ADC_Init(&hadc1);

	/**Configure the ADC multi-mode 
	*/
	multimode.Mode = ADC_TRIPLEMODE_INTERL;
	multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
	multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
	*/
	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_xCYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

}

/* ADC2 init function */
static void MX_ADC2_Init (void) {

	ADC_MultiModeTypeDef multimode;
	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
	*/
	hadc2.Instance = ADC2;
	hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc2.Init.Resolution = ADC_RESOLUTION_12B;
	hadc2.Init.ScanConvMode = DISABLE;
	hadc2.Init.ContinuousConvMode = DISABLE;
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc2.Init.NbrOfConversion = 1;
	hadc2.Init.DMAContinuousRequests = DISABLE;
	hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	HAL_ADC_Init(&hadc2);

	/**Configure the ADC multi-mode 
	*/
	multimode.Mode = ADC_TRIPLEMODE_INTERL;
	multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
	multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	HAL_ADCEx_MultiModeConfigChannel(&hadc2, &multimode);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
	*/
	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_xCYCLES;
	HAL_ADC_ConfigChannel(&hadc2, &sConfig);

}

/* ADC3 init function */
static void MX_ADC3_Init (void) {

	ADC_MultiModeTypeDef multimode;
	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
	*/
	hadc3.Instance = ADC3;
	hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc3.Init.Resolution = ADC_RESOLUTION_12B;
	hadc3.Init.ScanConvMode = DISABLE;
	hadc3.Init.ContinuousConvMode = DISABLE;
	hadc3.Init.DiscontinuousConvMode = DISABLE;
	hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc3.Init.NbrOfConversion = 1;
	hadc3.Init.DMAContinuousRequests = DISABLE;
	hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	HAL_ADC_Init(&hadc3);

	/**Configure the ADC multi-mode 
	*/
	multimode.Mode = ADC_TRIPLEMODE_INTERL;
	multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
	multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	HAL_ADCEx_MultiModeConfigChannel(&hadc3, &multimode);

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
	*/
	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_xCYCLES;
	HAL_ADC_ConfigChannel(&hadc3, &sConfig);

}

/*  ---------------------------------------------------------*/
void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef* hadc) {

}

/*  ---------------------------------------------------------*/
void MX_DMA_ADC_Init (void) {
	/* ADC ---------------------------------------------------------*/
	/* DMA2_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

