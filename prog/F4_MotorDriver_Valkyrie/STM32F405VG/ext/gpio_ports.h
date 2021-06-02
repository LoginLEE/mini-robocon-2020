#ifndef __GPIO_PORTS_H
#define __GPIO_PORTS_H

#include "gpio.h"

#define GPIO_PORTS_TABLE \
	X(GPIO_PORT_1,  PE0) \
	X(GPIO_PORT_2,  PE1) \
	X(GPIO_PORT_3,  PE2) \
	X(GPIO_PORT_4,  PE3) \
	X(GPIO_PORT_5,  PE4) \
	X(GPIO_PORT_6,  PE5) \
	X(GPIO_PORT_7,  PE6) \
	X(GPIO_PORT_8,  PE7) \
	X(GPIO_PORT_9,  PE8) \
	X(GPIO_PORT_10, PE9) \
	X(GPIO_PORT_11, PE10) \
	X(GPIO_PORT_12, PE11) \
	X(GPIO_PORT_13, PE12) \
	X(GPIO_PORT_14, PE13) \
	X(GPIO_PORT_15, PE14) \
	X(GPIO_PORT_16, PE15) \
	X(GPIO_PORT_17, PG0) \
	X(GPIO_PORT_18, PG1) \
	X(GPIO_PORT_19, PG2) \
	X(GPIO_PORT_20, PG3) \
	X(GPIO_PORT_21, PG4) \
	X(GPIO_PORT_22, PG15) \
	X(GPIO_PORT_23, PF15) \
	X(GPIO_PORT_24, PB7)

#define GPIO12V_PORTS_TABLE \
	X(GPIO12V_PORT_1, PF3) \
	X(GPIO12V_PORT_2, PF4) \
	X(GPIO12V_PORT_3, PF5) \
	X(GPIO12V_PORT_4, PF6) \
	X(GPIO12V_PORT_5, PC0) \
	X(GPIO12V_PORT_6, PC1) \
	X(GPIO12V_PORT_7, PC2) \
	X(GPIO12V_PORT_8, PC3) \
	X(GPIO12V_PORT_9, PC4) \
	X(GPIO12V_PORT_10, PC5) 

#define X(a, b) a,
typedef enum {
	GPIO_PORTS_TABLE
} GpioPortID;
#undef X

#define X(a, b) a,
typedef enum {
	GPIO12V_PORTS_TABLE
} Gpio12VPortID;
#undef X

#define X(a, b) &b,
static const GPIO* GpioPorts[] = {GPIO_PORTS_TABLE};
#undef X

#define X(a, b) &b,
static const GPIO* Gpio12VPorts[] = {GPIO12V_PORTS_TABLE};
#undef X

#define GPIO_PORT_COUNT    (sizeof(GpioPorts)   /sizeof(GPIO*))
#define GPIO12V_PORT_COUNT (sizeof(Gpio12VPorts)/sizeof(GPIO*))

#endif
