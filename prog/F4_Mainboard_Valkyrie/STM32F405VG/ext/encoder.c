#include "encoder.h"

static volatile EncoderStruct enc[2];

//Init encoder
void encoder_1_init(EncoderInputMode mode)
{
	enc[0].mode = mode;
	//--AVBZG Encoder Mode--
	if(mode == ENC)
	{
		//TIM2 af
		// CH1 - PA15
		// CH2 - PB3
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);
			
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_StructInit(&GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//open drain or push pull?
			//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_StructInit(&GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//open drain or push pull??
			//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			TIM_TimeBaseStructure.TIM_Prescaler = 0;
			TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
			
			TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
			
			TIM_SetCounter(TIM2, 0);
			TIM_Cmd(TIM2, ENABLE);
		}
		
		//TIM3 af
		// CH1 - PB4
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
			
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_StructInit(&GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//open drain or push pull?
			//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			TIM_TimeBaseStructure.TIM_Prescaler = 0;
			TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
			
			TIM_TIxExternalClockConfig(TIM3,TIM_TIxExternalCLK1Source_TI1,TIM_ICPolarity_Rising,0x0);
			
			TIM_SetCounter(TIM3, 0);
			TIM_Cmd(TIM3, ENABLE);
		}
	}
	//**********************
	
	
	//--PWM Input Mode--
	else
	{
		//TIM3 af
		// CH1 - PB4
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
			
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
			
			NVIC_InitTypeDef NVIC_InitStructure;
			NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
			TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			TIM_TimeBaseStructure.TIM_Prescaler = 168/2 - 1;
			TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
			TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
			
			TIM_ICInitTypeDef TIM_ICInitStructure;
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
			TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
			TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
			TIM_ICInitStructure.TIM_ICFilter = 0x0;
			TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
			
			TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
			TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
			TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);
			TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
			TIM_Cmd(TIM3, ENABLE);
		}
		
		//reference:
		//http://wiki.csie.ncku.edu.tw/embedded/PWM#pwm-input-mode
		//https://community.st.com/s/question/0D50X00009XkhaQSAR/stm32-discovery-rc-receiver-pwm-input
	}
}
void encoder_2_init(EncoderInputMode mode)
{
	enc[1].mode = mode;
	//--AVBZG Encoder Mode--
	if(mode == ENC)
	{
		//TIM5 af
		// CH1 - PA0
		// CH2 - PA1
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
			
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_StructInit(&GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
			GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			TIM_TimeBaseStructure.TIM_Prescaler = 0;
			TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
			
			TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
			
			TIM_SetCounter(TIM5, 0);
			TIM_Cmd(TIM5, ENABLE);
		}
		
		//TIM9 af
		// CH1 - PA2
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);
			
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_StructInit(&GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//open drain or push pull?
			//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			TIM_TimeBaseStructure.TIM_Prescaler = 0;
			TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);
			
			TIM_TIxExternalClockConfig(TIM9,TIM_TIxExternalCLK1Source_TI1,TIM_ICPolarity_Rising,0x0);
			
			TIM_SetCounter(TIM9, 0);
			TIM_Cmd(TIM9, ENABLE);
		}
	}
	//**********************
	
	
	//--PWM Input Mode--
	else
	{
		//TIM9 af
		// CH1 - PA2
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);
			
			NVIC_InitTypeDef NVIC_InitStructure;
			NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
			TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;
			TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
			TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);
			
			TIM_ICInitTypeDef TIM_ICInitStructure;
			TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
			TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
			TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
			TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
			TIM_ICInitStructure.TIM_ICFilter = 0x0;
			TIM_PWMIConfig(TIM9, &TIM_ICInitStructure);
			
			TIM_SelectInputTrigger(TIM9, TIM_TS_TI1FP1);
			TIM_SelectSlaveMode(TIM9, TIM_SlaveMode_Reset);
			TIM_SelectMasterSlaveMode(TIM9, TIM_MasterSlaveMode_Enable);
			TIM_ITConfig(TIM9, TIM_IT_CC1, ENABLE);
			TIM_Cmd(TIM9, ENABLE);
		}
		
		//reference:
		//http://wiki.csie.ncku.edu.tw/embedded/PWM#pwm-input-mode
		//https://community.st.com/s/question/0D50X00009XkhaQSAR/stm32-discovery-rc-receiver-pwm-input
	}
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		if(enc[0].mode == PWM)
		{
			enc[0].r1 = TIM_GetCapture1(TIM3);
			enc[0].r2 = TIM_GetCapture2(TIM3) + 1;
		}
	}
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM9, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);
		if(enc[1].mode == PWM)
		{
			enc[1].r1 = TIM_GetCapture1(TIM9);
			enc[1].r2 = TIM_GetCapture2(TIM9) + 1;
		}
	}
}

void encoder_1_update()
{
	if(enc[0].mode == ENC)
	{
		enc[0].r1 += TIM_GetCounter(TIM2);
		TIM_SetCounter(TIM2, 0);
	}
}
void encoder_2_update()
{
	if(enc[0].mode == ENC)
	{
		enc[1].r1 += TIM_GetCounter(TIM5);
		TIM_SetCounter(TIM5, 0);
		
	}
}

EncoderStruct get_encoder_reading(EncoderID id)
{
	return enc[id];
}

void reset_encoder(EncoderID id)
{
	enc[id].r1 = 0;
	enc[id].r2 = 0;
	if(id == Encoder1)
		TIM_SetCounter(TIM2, 0);
	if(id == Encoder2)
		TIM_SetCounter(TIM5, 0);
}
