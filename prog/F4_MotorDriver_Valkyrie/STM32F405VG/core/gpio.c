/**
	A gpio library for easier usage of GPIO ports
	Refreshed in F4 by Rex Cheng
*/

#include "gpio.h"

const GPIO 	
			/*** GPIOA ***/
			PA0 = {GPIOA, GPIO_Pin_0},
			PA1 = {GPIOA, GPIO_Pin_1},
			PA2 = {GPIOA, GPIO_Pin_2},
			PA3 = {GPIOA, GPIO_Pin_3},
			PA4 = {GPIOA, GPIO_Pin_4},
			PA5 = {GPIOA, GPIO_Pin_5},
			PA6 = {GPIOA, GPIO_Pin_6},
			PA7 = {GPIOA, GPIO_Pin_7},
			PA8 = {GPIOA, GPIO_Pin_8},
			PA9 = {GPIOA, GPIO_Pin_9},
			PA10 = {GPIOA, GPIO_Pin_10},
			PA11 = {GPIOA, GPIO_Pin_11},
			PA12 = {GPIOA, GPIO_Pin_12},
			PA13 = {GPIOA, GPIO_Pin_13},
			PA14 = {GPIOA, GPIO_Pin_14},
			PA15 = {GPIOA, GPIO_Pin_15},

			/*** GPIOB ***/
			PB0 = {GPIOB, GPIO_Pin_0},
			PB1 = {GPIOB, GPIO_Pin_1},
			PB2 = {GPIOB, GPIO_Pin_2},
			PB3 = {GPIOB, GPIO_Pin_3},
			PB4 = {GPIOB, GPIO_Pin_4},
			PB5 = {GPIOB, GPIO_Pin_5},
			PB6 = {GPIOB, GPIO_Pin_6},
			PB7 = {GPIOB, GPIO_Pin_7},
			PB8 = {GPIOB, GPIO_Pin_8},
			PB9 = {GPIOB, GPIO_Pin_9},
			PB10 = {GPIOB, GPIO_Pin_10},
			PB11 = {GPIOB, GPIO_Pin_11},
			PB12 = {GPIOB, GPIO_Pin_12},
			PB13 = {GPIOB, GPIO_Pin_13},
			PB14 = {GPIOB, GPIO_Pin_14},
			PB15 = {GPIOB, GPIO_Pin_15},

			/*** GPIOC ***/
			PC0 = {GPIOC, GPIO_Pin_0},
			PC1 = {GPIOC, GPIO_Pin_1},
			PC2 = {GPIOC, GPIO_Pin_2},
			PC3 = {GPIOC, GPIO_Pin_3},
			PC4 = {GPIOC, GPIO_Pin_4},
			PC5 = {GPIOC, GPIO_Pin_5},
			PC6 = {GPIOC, GPIO_Pin_6},
			PC7 = {GPIOC, GPIO_Pin_7},
			PC8 = {GPIOC, GPIO_Pin_8},
			PC9 = {GPIOC, GPIO_Pin_9},
			PC10 = {GPIOC, GPIO_Pin_10},
			PC11 = {GPIOC, GPIO_Pin_11},
			PC12 = {GPIOC, GPIO_Pin_12},
			PC13 = {GPIOC, GPIO_Pin_13},
			PC14 = {GPIOC, GPIO_Pin_14},
			PC15 = {GPIOC, GPIO_Pin_15},
			
			/*** GPIOD ***/
			PD0 = {GPIOD, GPIO_Pin_0},
			PD1 = {GPIOD, GPIO_Pin_1},
			PD2 = {GPIOD, GPIO_Pin_2},
			PD3 = {GPIOD, GPIO_Pin_3},
			PD4 = {GPIOD, GPIO_Pin_4},
			PD5 = {GPIOD, GPIO_Pin_5},
			PD6 = {GPIOD, GPIO_Pin_6},
			PD7 = {GPIOD, GPIO_Pin_7},
			PD8 = {GPIOD, GPIO_Pin_8},
			PD9 = {GPIOD, GPIO_Pin_9},
			PD10 = {GPIOD, GPIO_Pin_10},
			PD11 = {GPIOD, GPIO_Pin_11},
			PD12 = {GPIOD, GPIO_Pin_12},
			PD13 = {GPIOD, GPIO_Pin_13},
			PD14 = {GPIOD, GPIO_Pin_14},
			PD15 = {GPIOD, GPIO_Pin_15},

			/*** GPIOE ***/
			PE0 = {GPIOE, GPIO_Pin_0},
			PE1 = {GPIOE, GPIO_Pin_1},
			PE2 = {GPIOE, GPIO_Pin_2},
			PE3 = {GPIOE, GPIO_Pin_3},
			PE4 = {GPIOE, GPIO_Pin_4},
			PE5 = {GPIOE, GPIO_Pin_5},
			PE6 = {GPIOE, GPIO_Pin_6},
			PE7 = {GPIOE, GPIO_Pin_7},
			PE8 = {GPIOE, GPIO_Pin_8},
			PE9 = {GPIOE, GPIO_Pin_9},
			PE10 = {GPIOE, GPIO_Pin_10},
			PE11 = {GPIOE, GPIO_Pin_11},
			PE12 = {GPIOE, GPIO_Pin_12},
			PE13 = {GPIOE, GPIO_Pin_13},
			PE14 = {GPIOE, GPIO_Pin_14},
			PE15 = {GPIOE, GPIO_Pin_15},

			/*** GPIOF ***/
			PF0 = {GPIOF, GPIO_Pin_0},
			PF1 = {GPIOF, GPIO_Pin_1},
			PF2 = {GPIOF, GPIO_Pin_2},
			PF3 = {GPIOF, GPIO_Pin_3},
			PF4 = {GPIOF, GPIO_Pin_4},
			PF5 = {GPIOF, GPIO_Pin_5},
			PF6 = {GPIOF, GPIO_Pin_6},
			PF7 = {GPIOF, GPIO_Pin_7},
			PF8 = {GPIOF, GPIO_Pin_8},
			PF9 = {GPIOF, GPIO_Pin_9},
			PF10 = {GPIOF, GPIO_Pin_10},
			PF11 = {GPIOF, GPIO_Pin_11},
			PF12 = {GPIOF, GPIO_Pin_12},
			PF13 = {GPIOF, GPIO_Pin_13},
			PF14 = {GPIOF, GPIO_Pin_14},
			PF15 = {GPIOF, GPIO_Pin_15},

			/*** GPIOG ***/
			PG0 = {GPIOG, GPIO_Pin_0},
			PG1 = {GPIOG, GPIO_Pin_1},
			PG2 = {GPIOG, GPIO_Pin_2},
			PG3 = {GPIOG, GPIO_Pin_3},
			PG4 = {GPIOG, GPIO_Pin_4},
			PG5 = {GPIOG, GPIO_Pin_5},
			PG6 = {GPIOG, GPIO_Pin_6},
			PG7 = {GPIOG, GPIO_Pin_7},
			PG8 = {GPIOG, GPIO_Pin_8},
			PG9 = {GPIOG, GPIO_Pin_9},
			PG10 = {GPIOG, GPIO_Pin_10},
			PG11 = {GPIOG, GPIO_Pin_11},
			PG12 = {GPIOG, GPIO_Pin_12},
			PG13 = {GPIOG, GPIO_Pin_13},
			PG14 = {GPIOG, GPIO_Pin_14},
			PG15 = {GPIOG, GPIO_Pin_15}
			;

//EXTI0_IRQHandler
//EXTI1_IRQHandler
//EXTI2_IRQHandler
//EXTI3_IRQHandler
//EXTI4_IRQHandler
__weak void EXTI5_IRQHandler(void) {}
__weak void EXTI6_IRQHandler(void) {}
__weak void EXTI7_IRQHandler(void) {}
__weak void EXTI8_IRQHandler(void) {}
__weak void EXTI9_IRQHandler(void) {}
__weak void EXTI10_IRQHandler(void) {}
__weak void EXTI11_IRQHandler(void) {}
__weak void EXTI12_IRQHandler(void) {}
__weak void EXTI13_IRQHandler(void) {}
__weak void EXTI14_IRQHandler(void) {}
__weak void EXTI15_IRQHandler(void) {}

static inline IRQn_Type get_exti_group(u16 pin) {
	switch (pin) {
		case GPIO_Pin_0: return EXTI0_IRQn;
		case GPIO_Pin_1: return EXTI1_IRQn;
		case GPIO_Pin_2: return EXTI2_IRQn;
		case GPIO_Pin_3: return EXTI3_IRQn;
		case GPIO_Pin_4: return EXTI4_IRQn;
		case GPIO_Pin_5: 
		case GPIO_Pin_6: 
		case GPIO_Pin_7: 
		case GPIO_Pin_8: 
		case GPIO_Pin_9: return EXTI9_5_IRQn;
		case GPIO_Pin_10: 
		case GPIO_Pin_11: 
		case GPIO_Pin_12: 
		case GPIO_Pin_13: 
		case GPIO_Pin_14: 
		case GPIO_Pin_15: return EXTI15_10_IRQn;
	};
}

static inline u8 get_port_source(GPIO_TypeDef* port) {
	switch ((u32) port) {
		case GPIOA_BASE: return EXTI_PortSourceGPIOA;
		case GPIOB_BASE: return EXTI_PortSourceGPIOB;
		case GPIOC_BASE: return EXTI_PortSourceGPIOC;
		case GPIOD_BASE: return EXTI_PortSourceGPIOD;
	};
}

static inline u16 get_pin_source(u16 pin) {
	u8 pin_source = 0;
	do {
		if (pin & 0x01) break;
		pin >>=1;
		pin_source++;
	} while (1);
	return pin_source;
}

void gpio_exti_init(const GPIO* gpio, EXTITrigger_TypeDef trigger) {
	IRQn_Type exti_grp = get_exti_group(gpio->pin);
	u8 port_source = get_port_source(gpio->port);
	u8 pin_source = get_pin_source(gpio->pin);

	//	NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = exti_grp;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//	EXTI
	SYSCFG_EXTILineConfig(port_source, pin_source);
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = gpio->pin;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = trigger; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void gpio_af_init(const GPIO* gpio, GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd, u8 af) {
	gpio_init(gpio, GPIO_Mode_AF, otype, pupd);
	GPIO_PinAFConfig(gpio->port, get_pin_source(gpio->pin), af);
}

void EXTI9_5_IRQHandler(void) {
	if ((EXTI->PR & EXTI_Line5) != RESET)
	{
		EXTI5_IRQHandler();
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if ((EXTI->PR & EXTI_Line6) != RESET)
	{
		EXTI6_IRQHandler();
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if ((EXTI->PR & EXTI_Line7) != RESET)
	{
		EXTI7_IRQHandler();
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if ((EXTI->PR & EXTI_Line8) != RESET)
	{
		EXTI8_IRQHandler();
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if ((EXTI->PR & EXTI_Line9) != RESET)
	{
		EXTI9_IRQHandler();
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	if ((EXTI->PR & EXTI_Line10) != RESET)
	{
		EXTI10_IRQHandler();
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if ((EXTI->PR & EXTI_Line11) != RESET)
	{
		EXTI11_IRQHandler();
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if ((EXTI->PR & EXTI_Line12) != RESET)
	{
		EXTI12_IRQHandler();
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if ((EXTI->PR & EXTI_Line13) != RESET)
	{
		EXTI13_IRQHandler();
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if ((EXTI->PR & EXTI_Line14) != RESET)
	{
		EXTI14_IRQHandler();
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if ((EXTI->PR & EXTI_Line15) != RESET)
	{
		EXTI15_IRQHandler();
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

/**
	General input GPIO initailizer
	Usage: @ref gpio_init
	Output type and speed does not matter to input gpio
*/
void gpio_input_init(const GPIO* gpio, GPIOPuPd_TypeDef pp_type){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = gpio->pin;
	GPIO_InitStructure.GPIO_PuPd = pp_type;
	
	GPIO_Init(gpio->port, &GPIO_InitStructure);
}

/**
	General output GPIO initailizer
	Usage: @ref gpio_init
	Speed is fixed to GPIO_Medium_Speed which should be sufficient.
*/
void gpio_output_init(const GPIO* gpio, GPIOOType_TypeDef output_type, GPIOPuPd_TypeDef pp_type){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; 
	GPIO_InitStructure.GPIO_Pin = gpio->pin;
	GPIO_InitStructure.GPIO_OType = output_type;
	GPIO_InitStructure.GPIO_PuPd = pp_type;
	
	GPIO_Init(gpio->port, &GPIO_InitStructure);
}

/**
	Initilize RCC clock for all GPIO ports
*/
void gpio_rcc_init_all(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
}

/**
	* @brief GPIO Real-time Clock Initialization
	* @param GPIO pointer
	*/
void gpio_rcc_init(const GPIO* gpio){
	switch ((uint32_t) gpio->port) {
		
		case ((uint32_t)GPIOA):
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		break;
		
		case ((uint32_t)GPIOB):
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		break;
		
		case ((uint32_t)GPIOC):
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		break;
		
		case ((uint32_t)GPIOD):
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		break;
		
		case ((uint32_t)GPIOE):
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		break;
		
		case ((uint32_t)GPIOF):
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		break;
		
		case ((uint32_t)GPIOG):
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
		break;
	}
}

uint16_t getPinSource(const GPIO* gpio){
	switch(gpio->pin){
		case GPIO_Pin_0:
			return GPIO_PinSource0;
		case GPIO_Pin_1:
			return GPIO_PinSource1;
		case GPIO_Pin_2:
			return GPIO_PinSource2;
		case GPIO_Pin_3:
			return GPIO_PinSource3;
		case GPIO_Pin_4:
			return GPIO_PinSource4;
		case GPIO_Pin_5:
			return GPIO_PinSource5;
		case GPIO_Pin_6:
			return GPIO_PinSource6;
		case GPIO_Pin_7:
			return GPIO_PinSource7;
		case GPIO_Pin_8:
			return GPIO_PinSource8;
		case GPIO_Pin_9:
			return GPIO_PinSource9;
		case GPIO_Pin_10:
			return GPIO_PinSource10;
		case GPIO_Pin_11:
			return GPIO_PinSource11;
		case GPIO_Pin_12:
			return GPIO_PinSource12;
		case GPIO_Pin_13:
			return GPIO_PinSource13;
		case GPIO_Pin_14:
			return GPIO_PinSource14;
		default:
			return GPIO_PinSource15;
	}
}
