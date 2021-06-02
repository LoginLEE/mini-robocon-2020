#include "button.h"

//Init buttons
void btn_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	for (uint8_t i=0; i<BTN_COUNT; i++){
		gpio_rcc_init(BUTTONS[i].gpio);
		gpio_input_init(BUTTONS[i].gpio, BUTTONS[i].PuPd);
	}
}

//Return the state of the selected button, 1 if pressed
uint8_t btn_pressed(ButtonID button_id)
{
	//For pull up, 1 is normal state, 0 is pressed
	if (BUTTONS[button_id].PuPd == GPIO_PuPd_UP){
		return !gpio_read(BUTTONS[button_id].gpio);
	}else{
		return gpio_read(BUTTONS[button_id].gpio);
	}
}

//Return the can id indicated by the switch
ValkyrieID get_can_id(void)
{
	return (ValkyrieID)((btn_pressed(CanID_1) << 2) + 
		                  (btn_pressed(CanID_2) << 1) +
	                    (btn_pressed(CanID_3) << 0));
}

void btn_update()
{
	//...
}
