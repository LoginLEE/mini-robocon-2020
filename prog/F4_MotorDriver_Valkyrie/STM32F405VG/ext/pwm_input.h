#ifndef __PWM_INPUT_H
#define __PWM_INPUT_H

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "gpio.h"

// DO NOT use multiple pins attached to the same TIM

// Timer, Channel, GPIO port, pin, RCC
#define PWM_TIMERS_TABLE(X) \
	X(PWM_IN_ENC_2_A, TIM3,  1, PC6,  1) \
	X(PWM_IN_ENC_2_B, TIM8,  2, PC7,  2) \
	X(PWM_IN_SERVO_1, TIM4,  1, PD12, 1) \
	X(PWM_IN_SERVO_2, TIM4,  2, PD13, 1) \
	X(PWM_IN_IO_PIN6, TIM9,  1, PE5,  2) \
	X(PWM_IN_IO_PIN7, TIM9,  2, PE6,  2) \
	X(PWM_IN_UART2_T, TIM9,  1, PA2,  2) \
	X(PWM_IN_UART2_R, TIM9,  2, PA3,  2) \
	X(PWM_IN_UART4_T, TIM5,  1, PA0,  1) \
	X(PWM_IN_UART4_R, TIM5,  2, PA1,  1) \
	X(PWM_IN_SPI2_MI, TIM12, 1, PB14, 1) \
	X(PWM_IN_SPI2_MO, TIM12, 2, PB15, 1) 

typedef struct {
	TIM_TypeDef* tim;
	u16 tim_ch;
	u16 tim_trig;
	u8 rcc_line;
	void (*rcc_cmd)(u32, FunctionalState);
	u32 rcc_tim;
	const GPIO* gpio;
	u8 gpio_af;
} PWM_IN_TypeDef;
#define X_INIT_STRUCTS(I, T, C, G, R) {T, TIM_Channel_ ## C, TIM_TS_TI ## C ## FP ## C, R, RCC_APB ## R ## PeriphClockCmd, RCC_APB ## R ## Periph_ ## T, &G, GPIO_AF_ ## T},
static const PWM_IN_TypeDef pwm_in_pins[] = {PWM_TIMERS_TABLE(X_INIT_STRUCTS)};

#define X_ID(I, T, C, G, R) I,
typedef enum {
	PWM_TIMERS_TABLE(X_ID)
} PWM_IN_ID;

void pwm_input_init(PWM_IN_ID id);
uint16_t read_pwm_input(PWM_IN_ID id);

#endif
