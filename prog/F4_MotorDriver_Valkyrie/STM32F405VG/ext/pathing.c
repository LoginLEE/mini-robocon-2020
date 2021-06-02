#include "pathing.h"
#include "motor_control.h"
u8 isWalking=1;
s16 step = 5;
void HC_walking(void);
void turningMode(void);
void app_motor_set_pos(MotorID id,int64_t myPos){
	motor_set_pos(id, myPos>>32, myPos);
}

void appLoop(void){
	HC_walking();
}
void HC_walking(void){
	if(isWalking){
		static int32_t theta2 = 0,theta3 = 300;
		app_motor_set_pos(Motor2, 3*int_cos(theta2)-10000/*pos2*/);
		app_motor_set_pos(Motor3, 3*int_sin(theta3)+5000/*pos3*/);
		//motor_set_vel(Motor2, 10*int_cos(theta2));
		//motor_set_vel(Motor3, 10*int_sin(theta3));
		if(theta2<1800){
			theta2+=15;	
			theta3+=15;
		}
		else{
			theta2+=5;
			theta3+=5;
		}
		if(theta3<180){
			
		}
		else{
			
		}
//		theta2+=5;
//		theta3+=5;
		if(theta2>=3600) theta2=0;
		if(theta3>=3600) theta3=0;
	}
}
void turningMode(void){
	static u16 cnt = 0;
	if(cnt<=2000){
		motor_set_vel(Motor3, 100000);
	}
	else{
		motor_set_vel(Motor3, 0);
		if(cnt>=4000) cnt=0;
	}
	cnt++;
}
/* End of libary */

