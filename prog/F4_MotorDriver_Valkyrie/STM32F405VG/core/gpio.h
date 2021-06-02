#ifndef	_GPIO_H
#define	_GPIO_H

/**
	A gpio library for easier usage of GPIO ports
	Refreshed in F4 by Rex Cheng
*/

#include "stm32f4xx_gpio.h"
#include <stdbool.h>

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} GPIO;

extern const GPIO 
	PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
	PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
	PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
	PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
	PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
	PG0, PG1, PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9, PG10, PG11, PG12, PG13, PG14, PG15;


extern const GPIO* GPIOArray[];
#define GPIO_SIZE (7*16)

/** Get the PinSourse from gpio
* @param A pointer to a GPIO const
* @return GPIO_PinSourceX
*/
uint16_t getPinSource(const GPIO* gpio);

static inline void gpio_init(const GPIO* gpio, GPIOMode_TypeDef mode, GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd) {
	// gpio_rcc_init(gpio->port);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_OType = otype;
	GPIO_InitStructure.GPIO_PuPd = pupd;
	GPIO_InitStructure.GPIO_Pin = gpio->pin;

	GPIO_Init(gpio->port, &GPIO_InitStructure);
}

/** General input GPIO initailizer
	Usage: @ref gpio_init
	Output type and speed does not matter to input gpio
*/
void gpio_input_init(const GPIO* gpio, GPIOPuPd_TypeDef pp_type);

/** General output GPIO initailizer
	Usage: @ref gpio_init
	Speed is fixed to GPIO_Medium_Speed which should be sufficient.
*/
void gpio_output_init(const GPIO* gpio, GPIOOType_TypeDef output_type, GPIOPuPd_TypeDef pp_type);

/**
	Alternate function GPIO initailizer
	Usage: @ref gpio_init
*/
void gpio_af_init(const GPIO* gpio, GPIOOType_TypeDef output_type, GPIOPuPd_TypeDef pp_type, uint8_t GPIO_AF);

/**
	Initilize RCC clock for all GPIO ports
*/
void gpio_rcc_init_all(void);

/** @brief GPIO Real-time Clock Initialization
	* @param GPIO pointer
	*/
void gpio_rcc_init(const GPIO* gpio);

/**
 * @brief      Read the input value of the GPIO Pin
 *
 * @param[in]  gpio  The GPIO Pin
 *
 * @return     { description_of_the_return_value }
 */
static inline u8 gpio_read(const GPIO* gpio) {
	return GPIO_ReadInputDataBit(gpio->port, gpio->pin);
}

/**
 * @brief      Write a value to the output of the GPIO Pin
 *
 * @param[in]  gpio  The GPIO Pin
 * @param[in]  bit   The output value
 */
static inline void gpio_write(const GPIO* gpio, u8 bit) {
	if (bit) GPIO_SetBits(gpio->port, gpio->pin);
	else     GPIO_ResetBits(gpio->port, gpio->pin);
}

/**
 * @brief      Quickly set the output of the GPIO Pin (High) functionally
 *             equivalent but sometimes faster than gpio_write(1)
 *
 * @param[in]  gpio  The GPIO Pin
 */
static inline void gpio_set(const GPIO* gpio) {
	GPIO_SetBits(gpio->port, gpio->pin);
}

/**
 * @brief      Quickly reset the output of the GPIO Pin (Low) functionally
 *             equivalent but sometimes faster than gpio_write(0)
 *
 * @param[in]  gpio  The GPIO Pin
 */
static inline void gpio_reset(const GPIO* gpio) {
	GPIO_ResetBits(gpio->port, gpio->pin);
}

static inline void gpio_toggle(const GPIO* gpio) {
	gpio->port->ODR ^= gpio->pin;
}

#endif
