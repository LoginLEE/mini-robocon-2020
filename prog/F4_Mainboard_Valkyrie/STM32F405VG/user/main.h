#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f4xx.h"
#include <stdlib.h>
#include "stm32f4xx_gpio.h"

#include "define.h"

#include "ticks.h"

//communication
#include "uart.h"
#include "can_protocol.h"
#include "spi_protocol.h"
#include "feedback.h"

//control
//#include "can_motor.h"
#include "gun_can_motor.h"
#include "encoder.h"
#include "motor_control.h"
#include "led.h"
#include "button.h"

//math library
#include "approx_math.h"
#include "macro_math.h"
#include "trigon_math.h"

//controller
#include "lcd_main.h"
#include "garuda_controller.h"

#endif 
