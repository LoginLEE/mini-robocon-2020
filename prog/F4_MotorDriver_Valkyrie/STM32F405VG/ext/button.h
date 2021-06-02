#ifndef	_BUTTON_H
#define	_BUTTON_H

#include "stm32f4xx.h"
#include <stdbool.h>
#include "define.h"
#include "gpio.h"

//BUTTON   PIN   Pull-up/Pull-down
#define BTN_TABLE \
X(Button1, PC15, GPIO_PuPd_UP) \
X(CanID_1, PC8,  GPIO_PuPd_UP) \
X(CanID_2, PC9,  GPIO_PuPd_UP) \
X(CanID_3, PA8,  GPIO_PuPd_UP)


#define X(a, b, c) a, 
typedef enum{
	BTN_TABLE
}ButtonID;
#undef X

typedef struct{
	const GPIO* gpio;
	const GPIOPuPd_TypeDef PuPd;
}ButtonStruct;

#define X(a, b, c) {&b, c},
static const ButtonStruct BUTTONS[] = {BTN_TABLE};
#undef X

#define BTN_COUNT (sizeof(BUTTONS)/sizeof(ButtonStruct))

typedef void(*OnClickListener)(void);
typedef void(*OnReleaseListener)(void);
typedef void(*OnHoldListener)(void);

typedef struct{
	OnHoldListener holdListener;
	s16 holdThreshold;
	s16 trigFrequency;
}HoldListenerStruct;

//Init buttons
void btn_init(void);

//Return the state of the selected button, 1 if pressed
uint8_t btn_pressed(ButtonID button_id);

//Return the can id indicated by the switch
ValkyrieID get_can_id(void);

//To be called at a regular time interval. Button event will be triggered in this function
void btn_update(void);

#endif
