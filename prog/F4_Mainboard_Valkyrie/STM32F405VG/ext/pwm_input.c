#include "pwm_input.h"

void pwm_input_init(PWM_IN_ID id) {
	const PWM_IN_TypeDef* init_struct = &pwm_in_pins[id];

	/* TIMx clock enable */
	(init_struct->rcc_cmd)(init_struct->rcc_tim, ENABLE);
	
	gpio_rcc_init(init_struct->gpio);
	gpio_af_init(init_struct->gpio, GPIO_OType_PP, GPIO_PuPd_NOPULL, init_struct->gpio_af);

	/* Enable the TIMx global Interrupt */
	// NVIC_InitTypeDef NVIC_InitStructure;
	// NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84*init_struct->rcc_line-1;

	TIM_TimeBaseInit(init_struct->tim, &TIM_TimeBaseInitStruct);

	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = init_struct->tim_ch==TIM_Channel_2 ? TIM_ICSelection_DirectTI : TIM_ICSelection_IndirectTI;
	TIM_PWMIConfig(init_struct->tim, &TIM_ICInitStructure);

	/* Select the TIMx Input Trigger */
	TIM_SelectInputTrigger(init_struct->tim, init_struct->tim_trig);

	/* Select the slave Mode: Reset Mode */
	TIM_SelectSlaveMode(init_struct->tim, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(init_struct->tim,TIM_MasterSlaveMode_Enable);

	/* TIM enable counter */
	TIM_Cmd(init_struct->tim, ENABLE);
}

uint16_t read_pwm_input(PWM_IN_ID id) {
	if (pwm_in_pins[id].tim_ch == TIM_Channel_1) return TIM_GetCapture1(pwm_in_pins[id].tim) - TIM_GetCapture2(pwm_in_pins[id].tim);
	else return TIM_GetCapture1(pwm_in_pins[id].tim);
}
