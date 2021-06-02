/********************************
// Motor driver - Project Valkyrie
// For Robomaster motor m3508 & esc c620
//
// Robocon 2019 Research & Development
// H K U S T
//
// Author : Emmett Yim
// Contact: yhyim@connect.ust.hk
// Github : emmettyim
// 
// Notes  :
//  1. Please let c620 tune its PID first WITHOUT load when changing esc / motor
//  2. LED signal
//   2.1 LED1 (Green) - Motor1 status
//   2.2 LED2 (Blue)  - Motor2 status
//   2.3 LED3 (White) - Motor3 status
//   2.4 LED4 (Red)   - Can1   status
//   2.5 Status reference
//    2.5.1 For motor
//             1. Flash (slow) - Normal, Alive
//             2. Flash (fast) - Stall
//             3. On           - Overheat
//             4. Off          - Dead / No CAN respond
//    2.5.2 For Can
//          ...need to update...xp
//  3. Implementation type
//   3.1 Normal (3 independent motor)
//       - All 3 motors can be used
//   3.2 Swerve drive
//       - Motor 1 as wheel
//       - Motor 3 as steering
//   3.3 Limb
//       - Motor 1&2 as joint
//       - Motor 3 as steering (if used)
// 
// Update :
// v0.5  Jan 2019 - (Converting testing into libraries)
// v0.9  Feb 2019 - (Running testing on PCB, converting libraries)
// v1.0  Mar 2019 - (First testing on wheelbase, controlling 3 motors at the same time)
//
*********************************/

/********************************
// Board checking
// ---------------
// 1.  SysTicks          ok
// 2.1 Leds - Green      ok
//   2 Leds - Blue       ok
//   3 Leds - White      ok
//   4 Leds - Red        ok
// 3.  Button            ok
// 4.1 Switch - Can ID 1 ok
//   2 Switch - Can ID 2 ok
//   3 Switch - Can ID 3 ok
//   4 Switch - Resistor ok
// 5.1 Uart1 - Tx        ok
//   2 Uart1 - Rx        ok
// 6.1 Uart1 - Tx        ok
//   2 Uart1 - Rx        ok
// 7.  Can1              ok
// 8.  Can2              ok
// 9.  SPI               ?
//10.1 ENC1 - Enc A&B    ?
//   2 ENC1 - Enc Z      ?
//   3 ENC1 - PWM input  ?
//11.1 ENC1 - Enc A&B    ?
//   2 ENC1 - Enc Z      ?
//   3 ENC1 - PWM input  ?
//12.  PWM1              ?
//13.  PWM2              ?
*********************************/
#include "main.h"

void crystal_status_detector(void);

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	ticks_init();
	led_init();
	btn_init();
	/*
	encoder_1_init(ENC);
	encoder_2_init(ENC);
	spi_init();*/
	feedback_init();
	motor_init(Motor1,RM3508);
	//motor_init(Motor2,RM2006);
	//motor_init(Motor3,RM2006);
	//can_init();
	//gun_can_motor_init();
	garuda_init(GAR_BT_FIRST);
	crystal_status_detector();
	//uart_init(COM1,115200);
	
	u32 last_ticks = 0;
	while(1) {
		//Main Program
		u32 this_ticks = get_ticks();
		if (this_ticks == last_ticks) continue;
		
		if(this_ticks > 3000)motor_set_vel(Motor1,-500000);
		
		static u32 last_wheel_ticks = 0;
		static s32 ready_pos = 0;
		if(this_ticks - last_wheel_ticks >= 10)
		{
				//motor_set_pos(Motor1,0,ready_pos);
				//motor_set_vel(Motor1,100000);
				//motor_set_vel(Motor2,0);
				//motor_set_vel(Motor3,0);
				
			last_wheel_ticks = this_ticks;
		}
		
		static u32 last_feedback_ticks = 0;
		if(this_ticks - last_feedback_ticks >= 1)
		{
			feedback_update();
			last_feedback_ticks = this_ticks;
		}
		
		static u32 last_led_ticks = 0;
		if (this_ticks - last_led_ticks >= 200){
			led_toggle(LED4);
			
			if(gar_get_connection() != GAR_BOTH_DISCONNECTED)
				led_toggle(LED1);
			else
				led_control(LED1,Bit_SET);
			
			if(motor_get_enc(Motor1))
				led_toggle(LED2);
			else
				led_control(LED2,Bit_SET);
			
			last_led_ticks = this_ticks;
		}
	}
}

void crystal_status_detector(void)
{
	led_control(LED1,Bit_SET);
	led_control(LED2,Bit_SET);
	led_control(LED3,Bit_SET);
	led_control(LED4,Bit_SET);
	while (get_ticks() < 10)
	{
		led_control(LED1,Bit_RESET);
		led_control(LED2,Bit_RESET);
		led_control(LED3,Bit_RESET);
		led_control(LED4,Bit_RESET);
	}
	//Cystal fucker detection
	if (IsClockFucked){
		while(1){
			led_control(LED1,Bit_RESET);
			led_control(LED2,Bit_RESET);
			led_control(LED3,Bit_RESET);
			led_control(LED4,Bit_RESET);
		}
	}
	led_control(LED1,Bit_SET);
	led_control(LED2,Bit_SET);
	led_control(LED3,Bit_SET);
	led_control(LED4,Bit_SET);
}
