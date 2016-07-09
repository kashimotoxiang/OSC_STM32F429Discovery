#include "sys.h"

//---------------------------------------------------------------------//
void delay_us (int nus) {
	u32 temp;
	SysTick->LOAD = 21 * nus;//168/8=21,8个tick一次systick
	SysTick->VAL = 0X00;//清空计数器
	SysTick->CTRL = 0X01;//使能，减到零是无动作，采用外部时钟源
	do {
		temp = SysTick->CTRL;//读取当前倒计数值
	}
	while ((temp & 0x01) && (!(temp & (1 << 16))));//等待时间到达
	SysTick->CTRL = 0x00; //关闭计数器
	SysTick->VAL = 0X00; //清空计数器
}

void delay_ms (int nms) {
	u32 temp;
	SysTick->LOAD = 21000 * nms;
	SysTick->VAL = 0X00;//清空计数器
	SysTick->CTRL = 0X01;//使能，减到零是无动作，采用外部时钟源
	do {
		temp = SysTick->CTRL;//读取当前倒计数值
	}
	while ((temp & 0x01) && (!(temp & (1 << 16))));//等待时间到达
	SysTick->CTRL = 0x00; //关闭计数器
	SysTick->VAL = 0X00; //清空计数器
}

void delay_25ns (int ns) {
	while (ns--);
	return;
}

//ÉèÖÃÏòÁ¿±íÆ«ÒÆµØÖ·
//NVIC_VectTab:»ùÖ·
//Offset:Æ«ÒÆÁ¿
void MY_NVIC_SetVectorTable (u32 NVIC_VectTab, u32 Offset) {
	SCB->VTOR = NVIC_VectTab | (Offset & (u32)0xFFFFFE00);//ÉèÖÃNVICµÄÏòÁ¿±íÆ«ÒÆ¼Ä´æÆ÷,VTORµÍ9Î»±£Áô,¼´[8:0]±£Áô¡£
}

//ÉèÖÃNVIC·Ö×é
//NVIC_Group:NVIC·Ö×é 0~4 ×Ü¹²5×é
void MY_NVIC_PriorityGroupConfig (u8 NVIC_Group) {
	u32 temp, temp1;
	temp1 = (~NVIC_Group) & 0x07;//È¡ºóÈýÎ»
	temp1 <<= 8;
	temp = SCB->AIRCR; //¶ÁÈ¡ÏÈÇ°µÄÉèÖÃ
	temp &= 0X0000F8FF; //Çå¿ÕÏÈÇ°·Ö×é
	temp |= 0X05FA0000; //Ð´ÈëÔ¿³×
	temp |= temp1;
	SCB->AIRCR = temp; //ÉèÖÃ·Ö×é
}

//ÉèÖÃNVIC
//NVIC_PreemptionPriority:ÇÀÕ¼ÓÅÏÈ¼¶
//NVIC_SubPriority       :ÏìÓ¦ÓÅÏÈ¼¶
//NVIC_Channel           :ÖÐ¶Ï±àºÅ
//NVIC_Group             :ÖÐ¶Ï·Ö×é 0~4
//×¢ÒâÓÅÏÈ¼¶²»ÄÜ³¬¹ýÉè¶¨µÄ×éµÄ·¶Î§!·ñÔò»áÓÐÒâÏë²»µ½µÄ´íÎó
//×é»®·Ö:
//×é0:0Î»ÇÀÕ¼ÓÅÏÈ¼¶,4Î»ÏìÓ¦ÓÅÏÈ¼¶
//×é1:1Î»ÇÀÕ¼ÓÅÏÈ¼¶,3Î»ÏìÓ¦ÓÅÏÈ¼¶
//×é2:2Î»ÇÀÕ¼ÓÅÏÈ¼¶,2Î»ÏìÓ¦ÓÅÏÈ¼¶
//×é3:3Î»ÇÀÕ¼ÓÅÏÈ¼¶,1Î»ÏìÓ¦ÓÅÏÈ¼¶
//×é4:4Î»ÇÀÕ¼ÓÅÏÈ¼¶,0Î»ÏìÓ¦ÓÅÏÈ¼¶
//NVIC_SubPriorityºÍNVIC_PreemptionPriorityµÄÔ­ÔòÊÇ,ÊýÖµÔ½Ð¡,Ô½ÓÅÏÈ
void MY_NVIC_Init (u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channel, u8 NVIC_Group) {
	u32 temp;
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//ÉèÖÃ·Ö×é
	temp = NVIC_PreemptionPriority << (4 - NVIC_Group);
	temp |= NVIC_SubPriority & (0x0f >> NVIC_Group);
	temp &= 0xf; //È¡µÍËÄÎ»
	NVIC->ISER[NVIC_Channel / 32] |= 1 << NVIC_Channel % 32;//Ê¹ÄÜÖÐ¶ÏÎ»(ÒªÇå³ýµÄ»°,ÉèÖÃICER¶ÔÓ¦Î»Îª1¼´¿É)
	NVIC->IP[NVIC_Channel] |= temp << 4; //ÉèÖÃÏìÓ¦ÓÅÏÈ¼¶ºÍÇÀ¶ÏÓÅÏÈ¼¶
}

//Íâ²¿ÖÐ¶ÏÅäÖÃº¯Êý
//Ö»Õë¶ÔGPIOA~I;²»°üÀ¨PVD,RTC,USB_OTG,USB_HS,ÒÔÌ«Íø»½ÐÑµÈ
//²ÎÊý:
//GPIOx:0~8,´ú±íGPIOA~I
//BITx:ÐèÒªÊ¹ÄÜµÄÎ»;
//TRIM:´¥·¢Ä£Ê½,1,ÏÂÉýÑØ;2,ÉÏ½µÑØ;3£¬ÈÎÒâµçÆ½´¥·¢
//¸Ãº¯ÊýÒ»´ÎÖ»ÄÜÅäÖÃ1¸öIO¿Ú,¶à¸öIO¿Ú,Ðè¶à´Îµ÷ÓÃ
//¸Ãº¯Êý»á×Ô¶¯¿ªÆô¶ÔÓ¦ÖÐ¶Ï,ÒÔ¼°ÆÁ±ÎÏß
void Ex_NVIC_Config (u8 GPIOx, u8 BITx, u8 TRIM) {
	u8 EXTOFFSET = (BITx % 4) * 4;
	RCC->APB2ENR |= 1 << 14; //Ê¹ÄÜSYSCFGÊ±ÖÓ
	SYSCFG->EXTICR[BITx / 4] &= ~(0x000F << EXTOFFSET);//Çå³ýÔ­À´ÉèÖÃ£¡£¡£¡
	SYSCFG->EXTICR[BITx / 4] |= GPIOx << EXTOFFSET; //EXTI.BITxÓ³Éäµ½GPIOx.BITx
	//×Ô¶¯ÉèÖÃ
	EXTI->IMR |= 1 << BITx; //¿ªÆôline BITxÉÏµÄÖÐ¶Ï(Èç¹ûÒª½ûÖ¹ÖÐ¶Ï£¬Ôò·´²Ù×÷¼´¿É)
	if (TRIM & 0x01)EXTI->FTSR |= 1 << BITx; //line BITxÉÏÊÂ¼þÏÂ½µÑØ´¥·¢
	if (TRIM & 0x02)EXTI->RTSR |= 1 << BITx; //line BITxÉÏÊÂ¼þÉÏÉý½µÑØ´¥·¢
}

//GPIO¸´ÓÃÉèÖÃ
//GPIOx:GPIOA~GPIOI.
//BITx:0~15,´ú±íIOÒý½Å±àºÅ.
//AFx:0~15,´ú±íAF0~AF15.
//AF0~15ÉèÖÃÇé¿ö(ÕâÀï½öÊÇÁÐ³ö³£ÓÃµÄ,ÏêÏ¸µÄÇë¼û407Êý¾ÝÊÖ²á,56Ò³Table 7):
//AF0:MCO/SWD/SWCLK/RTC   AF1:TIM1/TIM2;            AF2:TIM3~5;               AF3:TIM8~11
//AF4:I2C1~I2C3;          AF5:SPI1/SPI2;            AF6:SPI3;                 AF7:USART1~3;
//AF8:USART4~6;           AF9;CAN1/CAN2/TIM12~14    AF10:USB_OTG/USB_HS       AF11:ETH
//AF12:FSMC/SDIO/OTG/HS   AF13:DCIM                 AF14:                     AF15:EVENTOUT
void GPIO_AF_Set (GPIO_TypeDef* GPIOx, u8 BITx, u8 AFx) {
	GPIOx->AFR[BITx >> 3] &= ~(0X0F << ((BITx & 0X07) * 4));
	GPIOx->AFR[BITx >> 3] |= (u32)AFx << ((BITx & 0X07) * 4);
}

//GPIOÍ¨ÓÃÉèÖÃ
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,Î»ÉèÖÃ,Ã¿¸öÎ»´ú±íÒ»¸öIO,µÚ0Î»´ú±íPx0,µÚ1Î»´ú±íPx1,ÒÀ´ÎÀàÍÆ.±ÈÈç0X0101,´ú±íÍ¬Ê±ÉèÖÃPx0ºÍPx8.
//MODE:0~3;Ä£Ê½Ñ¡Ôñ,0,ÊäÈë(ÏµÍ³¸´Î»Ä¬ÈÏ×´Ì¬);1,ÆÕÍ¨Êä³ö;2,¸´ÓÃ¹¦ÄÜ;3,Ä£ÄâÊäÈë.
//OTYPE:0/1;Êä³öÀàÐÍÑ¡Ôñ,0,ÍÆÍìÊä³ö;1,¿ªÂ©Êä³ö.
//OSPEED:0~3;Êä³öËÙ¶ÈÉèÖÃ,0,2Mhz;1,25Mhz;2,50Mhz;3,100Mh.
//PUPD:0~3:ÉÏÏÂÀ­ÉèÖÃ,0,²»´øÉÏÏÂÀ­;1,ÉÏÀ­;2,ÏÂÀ­;3,±£Áô.
//×¢Òâ:ÔÚÊäÈëÄ£Ê½(ÆÕÍ¨ÊäÈë/Ä£ÄâÊäÈë)ÏÂ,OTYPEºÍOSPEED²ÎÊýÎÞÐ§!!
void GPIO_Set (GPIO_TypeDef* GPIOx, u32 BITx, u32 MODE, u32 OTYPE, u32 OSPEED, u32 PUPD) {
	u32 pinpos = 0, pos = 0, curpin = 0;
	for (pinpos = 0; pinpos < 16; pinpos++) {
		pos = 1 << pinpos; //Ò»¸ö¸öÎ»¼ì²é
		curpin = BITx & pos;//¼ì²éÒý½ÅÊÇ·ñÒªÉèÖÃ
		if (curpin == pos) //ÐèÒªÉèÖÃ
		{
			GPIOx->MODER &= ~(3 << (pinpos * 2)); //ÏÈÇå³ýÔ­À´µÄÉèÖÃ
			GPIOx->MODER |= MODE << (pinpos * 2); //ÉèÖÃÐÂµÄÄ£Ê½
			if ((MODE == 0X01) || (MODE == 0X02)) //Èç¹ûÊÇÊä³öÄ£Ê½/¸´ÓÃ¹¦ÄÜÄ£Ê½
			{
				GPIOx->OSPEEDR &= ~(3 << (pinpos * 2)); //Çå³ýÔ­À´µÄÉèÖÃ
				GPIOx->OSPEEDR |= (OSPEED << (pinpos * 2));//ÉèÖÃÐÂµÄËÙ¶ÈÖµ
				GPIOx->OTYPER &= ~(1 << pinpos); //Çå³ýÔ­À´µÄÉèÖÃ
				GPIOx->OTYPER |= OTYPE << pinpos; //ÉèÖÃÐÂµÄÊä³öÄ£Ê½
			}
			GPIOx->PUPDR &= ~(3 << (pinpos * 2)); //ÏÈÇå³ýÔ­À´µÄÉèÖÃ
			GPIOx->PUPDR |= PUPD << (pinpos * 2); //ÉèÖÃÐÂµÄÉÏÏÂÀ­
		}
	}
}

//THUMBÖ¸Áî²»Ö§³Ö»ã±àÄÚÁª
//²ÉÓÃÈçÏÂ·½·¨ÊµÏÖÖ´ÐÐ»ã±àÖ¸ÁîWFI
void WFI_SET (void) {
	__ASM volatile (

	"wfi"
	)
	;
}

//¹Ø±ÕËùÓÐÖÐ¶Ï(µ«ÊÇ²»°üÀ¨faultºÍNMIÖÐ¶Ï)
void INTX_DISABLE (void) {
	__ASM volatile (

	"cpsid i"
	)
	;
}

//¿ªÆôËùÓÐÖÐ¶Ï
void INTX_ENABLE (void) {
	__ASM volatile (

	"cpsie i"
	)
	;
}

//ÉèÖÃÕ»¶¥µØÖ·
//addr:Õ»¶¥µØÖ·
__asm
void MSR_MSP (u32 addr) {
	MSR MSP, r0 //set Main Stack value
	BX r14
}

//½øÈë´ý»úÄ£Ê½
void Sys_Standby (void) {
	SCB->SCR |= 1 << 2; //Ê¹ÄÜSLEEPDEEPÎ» (SYS->CTRL)
	RCC->APB1ENR |= 1 << 28;//Ê¹ÄÜµçÔ´Ê±ÖÓ
	PWR->CSR |= 1 << 8; //ÉèÖÃWKUPÓÃÓÚ»½ÐÑ
	PWR->CR |= 1 << 2; //Çå³ýWake-up ±êÖ¾
	PWR->CR |= 1 << 1; //PDDSÖÃÎ»
	WFI_SET(); //Ö´ÐÐWFIÖ¸Áî,½øÈë´ý»úÄ£Ê½
}

//ÏµÍ³Èí¸´Î»
void Sys_Soft_Reset (void) {
	SCB->AIRCR = 0X05FA0000 | (u32)0x04;
}

//Ê±ÖÓÉèÖÃº¯Êý
//Fvco=Fs*(plln/pllm);
//Fsys=Fvco/pllp=Fs*(plln/(pllm*pllp));
//Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));

//Fvco:VCOÆµÂÊ
//Fsys:ÏµÍ³Ê±ÖÓÆµÂÊ
//Fusb:USB,SDIO,RNGµÈµÄÊ±ÖÓÆµÂÊ
//Fs:PLLÊäÈëÊ±ÖÓÆµÂÊ,¿ÉÒÔÊÇHSI,HSEµÈ.
//plln:Ö÷PLL±¶ÆµÏµÊý(PLL±¶Æµ),È¡Öµ·¶Î§:64~432.
//pllm:Ö÷PLLºÍÒôÆµPLL·ÖÆµÏµÊý(PLLÖ®Ç°µÄ·ÖÆµ),È¡Öµ·¶Î§:2~63.
//pllp:ÏµÍ³Ê±ÖÓµÄÖ÷PLL·ÖÆµÏµÊý(PLLÖ®ºóµÄ·ÖÆµ),È¡Öµ·¶Î§:2,4,6,8.(½öÏÞÕâ4¸öÖµ!)
//pllq:USB/SDIO/Ëæ»úÊý²úÉúÆ÷µÈµÄÖ÷PLL·ÖÆµÏµÊý(PLLÖ®ºóµÄ·ÖÆµ),È¡Öµ·¶Î§:2~15.

//Íâ²¿¾§ÕñÎª8MµÄÊ±ºò,ÍÆ¼öÖµ:plln=336,pllm=8,pllp=2,pllq=7.
//µÃµ½:Fvco=8*(336/8)=336Mhz
//     Fsys=336/2=168Mhz
//     Fusb=336/7=48Mhz
//·µ»ØÖµ:0,³É¹¦;1,Ê§°Ü¡£
u8 Sys_Clock_Set (u32 plln, u32 pllm, u32 pllp, u32 pllq) {
	u16 retry = 0;
	u8 status = 0;
	RCC->CR |= 1 << 16; //HSE ¿ªÆô
	while (((RCC->CR & (1 << 17)) == 0) && (retry < 0X1FFF))retry++;//µÈ´ýHSE RDY
	if (retry == 0X1FFF)status = 1; //HSEÎÞ·¨¾ÍÐ÷
	else {
		RCC->APB1ENR |= 1 << 28; //µçÔ´½Ó¿ÚÊ±ÖÓÊ¹ÄÜ
		PWR->CR |= 3 << 14; //¸ßÐÔÄÜÄ£Ê½,Ê±ÖÓ¿Éµ½168Mhz
		RCC->CFGR |= (0 << 4) | (5 << 10) | (4 << 13);//HCLK ²»·ÖÆµ;APB1 4·ÖÆµ;APB2 2·ÖÆµ.
		RCC->CR &= ~(1 << 24); //¹Ø±ÕÖ÷PLL
		RCC->PLLCFGR = pllm | (plln << 6) | (((pllp >> 1) - 1) << 16) | (pllq << 24) | (1 << 22);//ÅäÖÃÖ÷PLL,PLLÊ±ÖÓÔ´À´×ÔHSE
		RCC->CR |= 1 << 24; //´ò¿ªÖ÷PLL
		while ((RCC->CR & (1 << 25)) == 0);//µÈ´ýPLL×¼±¸ºÃ
		FLASH->ACR |= 1 << 8; //Ö¸ÁîÔ¤È¡Ê¹ÄÜ.
		FLASH->ACR |= 1 << 9; //Ö¸ÁîcacheÊ¹ÄÜ.
		FLASH->ACR |= 1 << 10; //Êý¾ÝcacheÊ¹ÄÜ.
		FLASH->ACR |= 5 << 0; //5¸öCPUµÈ´ýÖÜÆÚ.
		RCC->CFGR &= ~(3 << 0); //ÇåÁã
		RCC->CFGR |= 2 << 0; //Ñ¡ÔñÖ÷PLL×÷ÎªÏµÍ³Ê±ÖÓ
		while ((RCC->CFGR & (3 << 2)) != (2 << 2));//µÈ´ýÖ÷PLL×÷ÎªÏµÍ³Ê±ÖÓ³É¹¦.
	}
	return status;
}

//ÏµÍ³Ê±ÖÓ³õÊ¼»¯º¯Êý
//plln:Ö÷PLL±¶ÆµÏµÊý(PLL±¶Æµ),È¡Öµ·¶Î§:64~432.
//pllm:Ö÷PLLºÍÒôÆµPLL·ÖÆµÏµÊý(PLLÖ®Ç°µÄ·ÖÆµ),È¡Öµ·¶Î§:2~63.
//pllp:ÏµÍ³Ê±ÖÓµÄÖ÷PLL·ÖÆµÏµÊý(PLLÖ®ºóµÄ·ÖÆµ),È¡Öµ·¶Î§:2,4,6,8.(½öÏÞÕâ4¸öÖµ!)
//pllq:USB/SDIO/Ëæ»úÊý²úÉúÆ÷µÈµÄÖ÷PLL·ÖÆµÏµÊý(PLLÖ®ºóµÄ·ÖÆµ),È¡Öµ·¶Î§:2~15.
void Stm32_Clock_Init (u32 plln, u32 pllm, u32 pllp, u32 pllq) {
	RCC->CR |= 0x00000001; //ÉèÖÃHISON,¿ªÆôÄÚ²¿¸ßËÙRCÕñµ´
	RCC->CFGR = 0x00000000; //CFGRÇåÁã
	RCC->CR &= 0xFEF6FFFF; //HSEON,CSSON,PLLONÇåÁã
	RCC->PLLCFGR = 0x24003010; //PLLCFGR»Ö¸´¸´Î»Öµ
	RCC->CR &= ~(1 << 18); //HSEBYPÇåÁã,Íâ²¿¾§Õñ²»ÅÔÂ·
	RCC->CIR = 0x00000000; //½ûÖ¹RCCÊ±ÖÓÖÐ¶Ï
	Sys_Clock_Set(plln, pllm, pllp, pllq);//ÉèÖÃÊ±ÖÓ
	//ÅäÖÃÏòÁ¿±í
#ifdef VECT_TAB_RAM
	MY_NVIC_SetVectorTable (1 << 29, 0x0);
#else
	MY_NVIC_SetVectorTable(0, 0x0);
#endif
}

