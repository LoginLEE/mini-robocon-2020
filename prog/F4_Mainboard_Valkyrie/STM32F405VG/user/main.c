/********************************
// Motor driver - Project Valkyrie
// For Robomaster motor m2006/m3508 & esc c610/c620
//
// Robocon 2019 Research & Development
// H K U S T
//
// Author : Emmett Yim
// Contact: yhyim@connect.ust.hk / 66816823
// Github : emmettyim
// 
// Notes  :
//  1. Please let c610/c620 tune its PID first WITHOUT load when changing esc / motor
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
//  3. Please visit <define.h> to access all the configurations & parameters
//  3. Implementation type
//   3.1 Normal (3 independent motor)
//       - All 3 motors can be used
//   3.2 Swerve drive
//       - Motor 1 as wheel
//       - Motor 3 as steering
//   3.3 Limb
//       - Motor 1&2 as joint
//       - Motor 3 as steering (if used)
//  4. Future Improvement
//   4.1 Configuration
//       - Initialization of the steering motor with abs. position feedback
//   4.2 Control
//       - Feedback & feedforward ctrl for steering motor
//   4.3 Communication
//       - SPI command & feedback
// 
// Update :
// v0.5  Jan 2019 - (Converting testing into libraries)
// v0.9  Feb 2019 - (Running testing on PCB, converting libraries)
// v1.0  Mar 2019 - (First testing on wheelbase, controlling 3 motors at the same time)
// v1.1  Jul 2019 - (Gen 1 function for swerve drive is finished)
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
//   3 ENC1 - PWM input  ok
//11.1 ENC1 - Enc A&B    ?
//   2 ENC1 - Enc Z      ?
//   3 ENC1 - PWM input  ok
//12.  PWM1              ?
//13.  PWM2              ?
*********************************/
#include "main.h"

void crystal_status_detector(void);
void id_indicator(void);

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	ticks_init();
	led_init();
	btn_init();
	encoder_1_init(PWM);
	encoder_2_init(PWM);
	//spi_init();
	feedback_init();
	motor_init(Motor1,RM3508);
	//motor_init(Motor2,RM3508);
	//motor_init(Motor3,RM3508);
	gun_can_motor_init();
	garuda_init(GAR_BT_FIRST);
	
	crystal_status_detector();
	id_indicator();
	
	u32 last_ticks = 0;
	while(1) {
		//Main Program
		u32 this_ticks = get_ticks();
		if (this_ticks == last_ticks) continue;
		
		static u32 last_wheel_ticks = 0;
		static u8 pressed1 = 0;
		static u8 pressed2 = 0;
		static s32 kick_pos = 0;
		static u8 ready = 0;
		if(this_ticks - last_wheel_ticks >= 10)
		{
			if(gar_get_connection() != GAR_BOTH_DISCONNECTED)
			{
				s16 vel = gar_get_joy(GAR_JOY_Y)*2;
				s16 steer = gar_get_joy(GAR_JOY_X);
				/*if(gar_button_pressed(GAR_UP_2) && !pressed1)
				{
					steer = 1500;
					pressed1 = 1;
				}
				else if(gar_button_pressed(GAR_DOWN_2) && !pressed1)
				{
					steer = -1500;
					pressed1 = 1;
				}
				else if(!gar_button_pressed(GAR_UP_2) && !gar_button_pressed(GAR_DOWN_2) && pressed1)
					pressed1 = 0;*/
				s16 left = vel - steer;
				s16 right = vel + steer;
				gun_can_motor_set_vel(MOTOR_1,left,Openloop);
				gun_can_motor_set_vel(MOTOR_2,-right,Openloop);
				
				if(gar_button_pressed(GAR_START_ZONE) && !pressed2)
				{
					kick_pos += 8192;
					pressed2 = 1;
					if(ready)
						motor_set_pos(Motor1,0,kick_pos);
				}
				else if(gar_button_pressed(GAR_LOAD_ZONE) && !pressed2)
				{
					kick_pos -= 8192;
					pressed2 = 1;
					if(ready)
						motor_set_pos(Motor1,0,kick_pos);
				}
				else if(!gar_button_pressed(GAR_START_ZONE) && !gar_button_pressed(GAR_LOAD_ZONE) && pressed2)
					pressed2 = 0;
				
				if(gar_button_pressed(GAR_SHOOT) && !pressed1)
				{
					if(ready)
					{
						ready = 0;
						if(ABS(motor_get_pos(Motor1)) > 100)
						{
							motor_set_accel(Motor1,7000);
							motor_set_vel(Motor1,RM3508_MaxVel);
						}
					}
					else
					{
						ready = 1;
						motor_set_accel(Motor1,2000);
						motor_set_pos(Motor1,0,kick_pos);
					}
					pressed1 = 1;
				}
				else if(!gar_button_pressed(GAR_SHOOT) && pressed1)
					pressed1 = 0;
				
				if(!ready && motor_get_pos(Motor1) >= -5)
				{
					motor_set_accel(Motor1,5000);
					motor_set_pos(Motor1,0,0);
				}
				
			}
			else
			{
				gun_can_motor_set_accel(MOTOR_1,9000000);
				gun_can_motor_set_accel(MOTOR_2,9000000);
				gun_can_motor_set_vel(MOTOR_1,0,Closeloop);
				gun_can_motor_set_vel(MOTOR_2,0,Closeloop);
				motor_set_vel_openloop(Motor1,0);
			}
			uart_motor_feedback_update();
			last_wheel_ticks = this_ticks;
		}
		
		static u32 last_led_ticks = 0;
		if (this_ticks - last_led_ticks >= 200){
			led_toggle(LED4);
			
			if(gar_get_connection() != GAR_BOTH_DISCONNECTED)
				led_toggle(LED1);
			else
				led_control(LED1,Bit_SET);
			
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
	while (get_ticks() < 100)
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

void id_indicator(void)
{
	led_control(LED1,Bit_SET);
	led_control(LED2,Bit_SET);
	led_control(LED3,Bit_SET);
	led_control(LED4,Bit_SET);
	while(get_ticks()<1000)
		valk_id_indicator(get_ticks());
	led_control(LED1,Bit_SET);
	led_control(LED2,Bit_SET);
	led_control(LED3,Bit_SET);
	led_control(LED4,Bit_SET);
}
