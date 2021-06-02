#include "motor_control.h"

extern volatile ValkyrieID ValkID;
extern volatile ValkCtrlMode valk_ctrl_mode;
volatile MotorStruct motor[MotorCnt];
volatile SwerveStruct swerve;
extern volatile MotorRawReadingStruct raw_reading[MotorCnt];

//init
void motor_basic_init(MotorID id)
{
	motor[id].state = 0x43;
	motor[id].alive_cnt = 0;
	motor[id].stall_cnt = 0;
	motor[id].torque = 0;
	motor[id].real_pos = 0;
	motor[id].cur_pos = 0;
	motor[id].tar_pos = 0;
	motor[id].real_vel = 0;
	motor[id].cur_vel = 0;
	motor[id].tar_vel = 0;
	motor[id].vel_cnt = 0;
	motor[id].pos_pid.err = 0;
	motor[id].pos_pid.c_err = 0;
	motor[id].pos_pid.p_err = 0;
	motor[id].vel_pid.err = 0;
	motor[id].vel_pid.c_err = 0;
	motor[id].vel_pid.p_err = 0;
	motor[id].rv = 0;
	motor[id].cv = 0;
}
void motor_init(MotorID id,MotorType mt)
{
	motor_basic_init(id);
	motor[id].mt = mt;
	switch(mt)
	{
		case RM3508:
			motor[id].overheat = RM3508_TEMP;
			motor[id].enc_cnt = RM3508_EncCnt;
			motor[id].max_vel = RM3508_MaxVel;
			motor[id].accel = RM3508_Accel;
			motor[id].pos_pid.kp = RM3508_POS_KP;
			motor[id].pos_pid.ki = RM3508_POS_KI;
			motor[id].pos_pid.kd = RM3508_POS_KD;
			motor[id].pos_pid.max_output = RM3508_VEL;
			motor[id].pos_pid.scaled_up = RM3508_SCALED;
			motor[id].vel_pid.kp = RM3508_VEL_KP;
			motor[id].vel_pid.ki = RM3508_VEL_KI;
			motor[id].vel_pid.kd = RM3508_VEL_KD;
			motor[id].vel_pid.kb = RM3508_VEL_KB;
			motor[id].vel_pid.kc = RM3508_VEL_KC;
			motor[id].vel_pid.max_output = RM3508_TORQUE;
			motor[id].vel_pid.scaled_up = RM3508_SCALED;
			break;
		case RM2006:
			motor[id].overheat = RM2006_TEMP;
			motor[id].enc_cnt = RM2006_EncCnt;
			motor[id].max_vel = RM2006_MaxVel;
			motor[id].accel = RM2006_Accel;
			motor[id].pos_pid.kp = RM2006_POS_KP;
			motor[id].pos_pid.ki = RM2006_POS_KI;
			motor[id].pos_pid.kd = RM2006_POS_KD;
			motor[id].pos_pid.max_output = RM2006_VEL;
			motor[id].pos_pid.scaled_up = RM2006_SCALED;
			motor[id].vel_pid.kp = RM2006_VEL_KP;
			motor[id].vel_pid.ki = RM2006_VEL_KI;
			motor[id].vel_pid.kd = RM2006_VEL_KD;
			motor[id].vel_pid.kb = RM2006_VEL_KB;
			motor[id].vel_pid.kc = RM2006_VEL_KC;
			motor[id].vel_pid.max_output = RM2006_TORQUE;
			motor[id].vel_pid.scaled_up = RM2006_SCALED;
			break;
	}
}
void motor_swerve_init(uint8_t enc_id,int32_t offset)
{
	if(valk_ctrl_mode != Swerve)
	{
		valk_ctrl_mode = Swerve;
		swerve.enc_used = (EncoderID)enc_id;
		if(swerve.enc_used == Encoder1)
			encoder_1_init(PWM);
		else
			encoder_2_init(PWM);
		swerve.offset = offset;
		swerve.a = -1;
	}
}

//path control
void motor_set_vel_openloop(MotorID id,int16_t torque)
{
	motor[id].state &= ~CloseloopMask;
	motor[id].state &= ~ReachedMask;
	motor[id].torque = torque;
}
void motor_set_vel(MotorID id,int32_t vel)
{
	if((motor[id].state & CloseloopMask) != CloseloopMask)
	{
		motor[id].state |= CloseloopMask;
		motor[id].tar_pos = motor[id].real_pos*1000;
		motor[id].cur_pos = motor[id].real_pos*1000;
	}
	if(motor[id].tar_vel != vel)
	{
		motor[id].state &= ~ReachedMask;
		motor[id].tar_vel = vel;
		motor[id].vel_cnt = -1;
	}
}
void motor_set_pos(MotorID id,int32_t hi,int32_t lo)
{
	int64_t pos = (((int64_t)hi)<<32)|lo;
	//uart_tx(UART_Other,"%d %d\n",hi,lo);
	pos *= 1000;
	if((motor[id].state & CloseloopMask) != CloseloopMask)
	{
		motor[id].state |= CloseloopMask;
		motor[id].tar_pos = motor[id].real_pos*1000;
		motor[id].cur_pos = motor[id].real_pos*1000;
	}
	if(motor[id].tar_pos != pos)
	{
		//uart_tx(UART_Motor,"c%d %d\n",motor[id].cur_vel,motor[id].cur_pos);
		motor[id].state &= ~ReachedMask;
		motor[id].tar_pos = pos;
		int64_t pos_diff = pos - motor[id].cur_pos;
		//if((motor[id].state & ReachedMask) != ReachedMask && motor[id].tar_vel == 0)
			//motor[id].cur_vel = 0;
		//int32_t to_zero = DIR(motor[id].cur_vel) * Sqrt((motor[id].cur_vel) * (motor[id].cur_vel) / (motor[id].accel))>>10;
		//uart_tx(UART_Motor,"p%d %d\n",pos,(int32_t)(pos_diff));
		int64_t to_zero_mag = (int64_t)(motor[id].cur_vel * (int64_t)(motor[id].cur_vel / motor[id].accel) / 2000);
		//uart_tx(UART_Motor,"m%d\n",to_zero_mag);
		int32_t to_zero = DIR(motor[id].cur_vel) * to_zero_mag;
		//uart_tx(UART_Motor,"t%d\n",to_zero);
		if(DIR(pos_diff) == DIR(motor[id].cur_vel) && ABS(pos_diff) > ABS(to_zero))
			pos_diff += to_zero*1000;
		else
			pos_diff -= to_zero*1000;
		//uart_tx(UART_Motor,"pd%d\n",(int32_t)(pos_diff));
		if(ABS(pos_diff) > ((int64_t)motor[id].max_vel)*motor[id].max_vel/motor[id].accel)
		{
			motor[id].tar_vel = DIR(pos_diff) * motor[id].max_vel;
			motor[id].vel_cnt = ABS(pos_diff - DIR(pos_diff) * ((int64_t)motor[id].max_vel)*motor[id].max_vel/motor[id].accel)/motor[id].max_vel;
			//uart_tx(UART_Motor,"v%d.%d\n",motor[id].tar_vel,motor[id].vel_cnt);
			motor[id].vel_cnt += ( ABS(pos_diff - DIR(pos_diff) * ((int64_t)motor[id].max_vel)*motor[id].max_vel/motor[id].accel) % motor[id].max_vel > motor[id].max_vel/2);
		}
		else
		{
			//int32_t = pos_diff * motor[id].accel / 1000;
			if(ABS(pos_diff)>motor[id].max_vel)
				motor[id].tar_vel = Sqrt((motor[id].accel/1000) * (pos_diff/1000))*1000>>10;
			else
				motor[id].tar_vel = Sqrt(motor[id].accel * pos_diff)*1000>>10;
			motor[id].tar_vel *= DIR(pos_diff);
			//uart_tx(UART_Motor,"v%d\n",motor[id].tar_vel);
			motor[id].vel_cnt = 0;
		}
	}
	//uart_tx(UART_Other,"%d %d\n",motor[id].tar_vel,motor[id].vel_cnt);
}
void motor_set_swerve(int32_t angle,int32_t vel)
{
	if(valk_ctrl_mode == Swerve)
	{
		swerve.v = vel;
		int64_t a = ((motor[Motor3].real_pos) * 3600 * RM2006_GearLo * Swerve_GearLo / RM2006_GearHi / Swerve_GearHi / RM2006_EncCnt + swerve.offset) % 3600;
		if(swerve.flipped)
			a = (a + 5400) % 3600;
		else
			a = (a + 3600) % 3600;
		int64_t angle_diff = (angle - a + 3600) % 3600;
		if(angle_diff > 1800)
			angle_diff -= 3600;
		if(ABS(angle_diff) > 900)
		{
			angle_diff -= DIR(angle_diff) * 1800;
			swerve.flipped = !swerve.flipped;
		}
		vel = ((int64_t)vel * int_cos(ABS(angle_diff)) / 10000);
		int32_t enc_diff = angle_diff * RM2006_EncCnt * RM2006_GearHi * Swerve_GearHi / Swerve_GearLo / RM2006_GearLo / 3600;
		if(swerve.a != angle)
		{
			swerve.a = angle;
			motor_set_pos(Motor3,(motor[Motor3].real_pos+enc_diff)>>32,motor[Motor3].real_pos+enc_diff);
		}
		if(swerve.flipped)
			motor_set_vel(Motor1,vel);
		else
			motor_set_vel(Motor1,-vel);
	}
}

void motor_set_accel(MotorID id,int32_t accel)
{
	motor[id].accel = accel;
}

int64_t motor_get_pos(MotorID id)
{
	return motor[id].real_pos;
}

//interupt
void motor_control_independent(MotorID id)
{
	//update encoder count
	static uint8_t init[MotorCnt];
	static uint16_t prev_enc[MotorCnt];
	int32_t d_enc = (motor[id].enc_cnt + raw_reading[id].enc - prev_enc[id]) % motor[id].enc_cnt;
	prev_enc[id] = raw_reading[id].enc;
	if(d_enc > motor[id].enc_cnt / 2)
		d_enc -= motor[id].enc_cnt;
	if(!init[id])
	{
		d_enc = 0;
		init[id] = 1;
	}
	motor[id].real_pos += d_enc;
	motor[id].real_vel = d_enc;
	motor[id].rv += d_enc;
	
	//closeloop control
	if((motor[id].state & CloseloopMask) == CloseloopMask)
	{
		//accel update
		if((motor[id].state & ReachedMask) != ReachedMask)
		{
			if(ABS(motor[id].tar_vel - motor[id].cur_vel) <= motor[id].accel)
			{
				motor[id].cur_vel = motor[id].tar_vel;
				if(motor[id].vel_cnt != -1)
				{
					if(motor[id].vel_cnt)
						motor[id].vel_cnt--;
					else if(motor[id].tar_vel)
						motor[id].tar_vel = 0;
					else
						motor[id].state |= ReachedMask;//??-------------------------------------------------- wrong?
				}
				else
					motor[id].state |= ReachedMask;
			}
			else
				motor[id].cur_vel += motor[id].accel * DIR(motor[id].tar_vel - motor[id].cur_vel);
			motor[id].cv += motor[id].cur_vel;
		}
		motor[id].cur_pos += motor[id].cur_vel;
		
		if(ABS(motor[id].tar_pos - motor[id].real_pos*1000) < motor[id].max_vel/5 && motor[id].tar_vel == 0 && (motor[id].state & ReachedMask) != ReachedMask && motor[id].vel_cnt <= 0)
		{//??------------------------------------------------max_vel / 5 to a smaller number?
			motor[id].cur_pos = motor[id].tar_pos;
			motor[id].cur_vel = motor[id].tar_vel;
			motor[id].state |= ReachedMask;
		}
		
		int32_t out_vel;
		motor[id].pos_pid.err = motor[id].cur_pos/1000 - motor[id].real_pos;
		if(motor[id].pos_pid.err > 0)
			out_vel = Sqrt(motor[id].pos_pid.err);
		else
			out_vel = -Sqrt(ABS(motor[id].pos_pid.err));
		out_vel = out_vel >> motor[id].pos_pid.scaled_up;
		out_vel = out_vel * motor[id].pos_pid.kp;
		out_vel += motor[id].pos_pid.c_err * motor[id].pos_pid.ki;
		out_vel += (motor[id].pos_pid.err - motor[id].pos_pid.p_err) * motor[id].pos_pid.kd;
		if(ABS(out_vel) > motor[id].pos_pid.max_output)
			out_vel = DIR(out_vel) * motor[id].pos_pid.max_output;
		motor[id].pos_pid.c_err += motor[id].pos_pid.err;
		motor[id].pos_pid.p_err = motor[id].pos_pid.err;
		
		//vel control
		motor[id].vel_pid.err = (motor[id].cur_vel + out_vel)/1000 - motor[id].real_vel;
		#ifdef VEL_2DOF
			motor[id].torque = (((motor[id].cur_vel + out_vel)/1000 * motor[id].vel_pid.kb >> motor[id].vel_pid.scaled_up) - motor[id].real_vel) * motor[id].vel_pid.kp;
			motor[id].torque += motor[id].vel_pid.c_err * motor[id].vel_pid.ki;
			motor[id].torque += ((motor[id].vel_pid.err * motor[id].vel_pid.kc >> motor[id].vel_pid.scaled_up) - motor[id].vel_pid.p_err) * motor[id].vel_pid.kd;
		#else
			motor[id].torque = motor[id].vel_pid.err * motor[id].vel_pid.kp;
			motor[id].torque += motor[id].vel_pid.c_err * motor[id].vel_pid.ki;
			motor[id].torque += (motor[id].vel_pid.err - motor[id].vel_pid.p_err) * motor[id].vel_pid.kd;
		#endif
		motor[id].torque = motor[id].torque >> motor[id].vel_pid.scaled_up;
		if(ABS(motor[id].torque) > motor[id].vel_pid.max_output)
			motor[id].torque = DIR(motor[id].torque) * motor[id].vel_pid.max_output;
		motor[id].vel_pid.c_err += motor[id].vel_pid.err;
		if(ABS(motor[id].vel_pid.c_err * motor[id].vel_pid.ki >> motor[id].vel_pid.scaled_up) > motor[id].vel_pid.max_output)
			motor[id].vel_pid.c_err = DIR(motor[id].vel_pid.c_err) * motor[id].vel_pid.max_output / motor[id].vel_pid.ki << motor[id].vel_pid.scaled_up;
		motor[id].vel_pid.p_err = motor[id].vel_pid.err;
	}
	//else
	//openloop control
	
	CanMessage msg;
	msg.id = CAN_RM_CtrlMask;	//C610/C620 ID 1 to 4
	msg.length = 8;
	msg.data[0] = motor[Motor1].torque>>8;
	msg.data[1] = motor[Motor1].torque;
	msg.data[2] = motor[Motor2].torque>>8;
	msg.data[3] = motor[Motor2].torque;
	msg.data[4] = motor[Motor3].torque>>8;
	msg.data[5] = motor[Motor3].torque;
	msg.data[6] = 0;
	msg.data[7] = 0;
	can_tx_enqueue(CAN_RM, msg);
}

void motor_control_steering()
{
	if(valk_ctrl_mode == Swerve)
	{
		//init
		//... but not in use for now
	}
}

//update
// runs @ 1kHz
void motor_status_update(void)
{
	static uint32_t last_led_ticks[MotorCnt];
	for(MotorID id = Motor1;id<MotorCnt;id++)
	{
		//alive checking
		if((motor[id].state & AliveMask) == AliveMask)
		{
			if(motor[id].alive_cnt > AliveTH)
			{
				motor_basic_init(id);
				raw_reading[id].enc = 0;
				raw_reading[id].vel = 0;
				raw_reading[id].atq = 0;
				raw_reading[id].tem = 0;
				motor[id].state &= ~AliveMask;
			}
			motor[id].alive_cnt++;
		}
		else//not alive
		{
			if(raw_reading[id].enc != 0 || raw_reading[id].vel != 0 || raw_reading[id].atq != 0 || raw_reading[id].tem != 0)
				motor_basic_init(id);
		}
		
		if((motor[id].state & AliveMask) == AliveMask)
		{
			//stall checking
			if((motor[id].state & StallMask) == StallMask)
			{
				if(motor[id].real_vel != 0)
					motor[id].state &= ~StallMask;
			}
			else
			{
				if((motor[id].state & ReachedMask) != ReachedMask && motor[id].real_vel == 0 && ABS(motor[id].torque) >= motor[id].vel_pid.max_output)
					motor[id].stall_cnt++;
				else if(motor[id].stall_cnt > 0)
					motor[id].stall_cnt--;
				if(motor[id].stall_cnt >= StallTH)
					motor[id].state |= StallMask;
			}
			
			//overheat checking
			if(raw_reading[id].tem >= motor[id].overheat)
				motor[id].state |= OverheatMask;
			else
				motor[id].state &= ~OverheatMask;
		}
		
		//led feedback
		if((motor[id].state & AliveMask) != AliveMask)
			led_control((LedID)id,Bit_SET);
		else
		{
			if((motor[id].state & StallMask) == StallMask)
			{
				if(get_ticks() - last_led_ticks[id] > 125)
				{
					led_toggle((LedID)id);
					last_led_ticks[id] = get_ticks();
				}
			}
			else
			{
				if(get_ticks() - last_led_ticks[id] > 250)
				{
					led_toggle((LedID)id);
					last_led_ticks[id] = get_ticks();
				}
			}
			if((motor[id].state & OverheatMask) == OverheatMask)
				led_control((LedID)id,Bit_RESET);
		}
	}
	//showing valk id
	if(get_ticks() < 1200)
	{
		led_control(LED1,!(BitAction)(ValkID>3));
		led_control(LED2,!(BitAction)(ValkID%4>1));
		led_control(LED3,!(BitAction)(ValkID%2));
	}
}
