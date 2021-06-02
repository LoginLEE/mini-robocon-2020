#ifndef	_LED_H
#define	_LED_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "gpio.h"

//LED1 - Motor1 (Green)
//LED2 - Motor2 (Blue)
//LED3 - Motor3 (White)
//LED4 - CAN    (Red)

#define LED_TABLE \
X(LED1, PB1)      \
X(LED2, PB0)      \
X(LED3, PC5)      \
X(LED4, PC4)      \

#define X(a, b) a,
typedef enum{
	LED_TABLE
} LedID;
#undef X

#define X(a, b) &b,
static const GPIO* LEDs[] = {LED_TABLE};
#undef X

#define LED_COUNT (sizeof(LEDs)/sizeof(GPIO*))

/**
	Initialize all LEDs.
*/
void led_init(void);

/**
	Control LED on/off
	@param id: the led to be controlled
	@param state: Bit_RESET: Off / Bit_SET: On
*/
void led_control(LedID id, BitAction state);

/**
	Make the led blink
	@param id: the led to be controlled
*/
void led_toggle(LedID id);

#endif
