#include "feedback.h"

volatile ValkyrieID ValkID;
volatile ValkCtrlMode valk_ctrl_mode;
CanFeedbackMode can_feedback_mode;
uint16_t alive_cnt = 0;
MotorID motor_selected;
volatile MotorRawReadingStruct raw_reading[MotorCnt];
extern volatile MotorStruct motor[MotorCnt];

void feedback_init(void)
{
	ValkID = get_can_id();
	can_feedback_mode = Sentinel;
	motor_selected = Motor1;
	//uart
	#ifndef UART_ECHO
		uart_init(UART_Motor,UART_BAUDRATE);
		uart_init(UART_Other,UART_BAUDRATE);
		uart_interrupt_init(UART_Motor,UART_Motor_RxHandler);
		uart_interrupt_init(UART_Other,UART_Other_RxHandler);
	#else
		uart_init(UART_Motor,UART_BAUDRATE);
		uart_init(UART_Other,UART_BAUDRATE);
		uart_interrupt_init(UART_Motor,UART_Motor_RxHandler);
		uart_interrupt_init(UART_Other,UART_Other_RxHandler);
	#endif
	//can
	can_init();
	can_rx_init();
	//can_rx_add_filter(CAN_MB_Mask,CAN_MB_RX_MASK,0,CAN_MB,CAN_MB_RxHandler);
	can_rx_add_filter(CAN_RM_RxMask1,CAN_RX_MASK_EXACT,0,CAN_RM,CAN_RM_RxHandler1);
	can_rx_add_filter(CAN_RM_RxMask2,CAN_RX_MASK_EXACT,0,CAN_RM,CAN_RM_RxHandler2);
	can_rx_add_filter(CAN_RM_RxMask3,CAN_RX_MASK_EXACT,0,CAN_RM,CAN_RM_RxHandler3);
}

void uart_motor_feedback_update()
{
	//uart_tx(UART_Motor,"%d %d\n",motor[motor_selected].rv,motor[motor_selected].cv/1000);
	motor[motor_selected].rv = 0;
	motor[motor_selected].cv = 0;
	//uart_tx(UART_Motor,"%d %d\n",motor[motor_selected].torque - ((int16_t)raw_reading[motor_selected].atq),raw_reading[motor_selected].tem);
	uart_tx(UART_Motor,"%d ",motor[motor_selected].real_pos);
	uart_tx(UART_Motor,"%d ",motor[motor_selected].cur_pos/1000);
	uart_tx(UART_Motor,"%d\n",motor[motor_selected].cur_vel);
}

void can_mb_feedback_update()
{
	static uint8_t last_motor_state[MotorCnt];
	CanMessage msg;
	msg.id = CAN_MB_FeedbackMask - ValkID;
	
	switch(can_feedback_mode)
	{
		case Passive:
			//if no request sent..............................................
			break;
		case Sentinel:
			if(motor[Motor1].state == last_motor_state[Motor1]&&
				 motor[Motor2].state == last_motor_state[Motor2]&&
				 motor[Motor3].state == last_motor_state[Motor3])
				break;
		case Continuous:
			if((motor[Motor1].state&AliveMask)==AliveMask || (motor[Motor1].state&FeedbackMask)==FeedbackMask)
			{
				msg.length = 8;
				msg.data[0] = Motor1;
				msg.data[1] = motor[Motor1].state;
				msg.data[2] = raw_reading[Motor1].enc>>8;
				msg.data[3] = raw_reading[Motor1].enc&0xF;
				msg.data[4] = raw_reading[Motor1].vel>>8;
				msg.data[5] = raw_reading[Motor1].vel&0xF;
				msg.data[6] = raw_reading[Motor1].atq>>8;
				msg.data[7] = raw_reading[Motor1].atq&0xF;
				can_tx_enqueue(CAN_RM,msg);
				msg.length = 8;
				msg.data[0] = motor[Motor1].real_pos>>56;
				msg.data[1] = motor[Motor1].real_pos>>48;
				msg.data[2] = motor[Motor1].real_pos>>40;
				msg.data[3] = motor[Motor1].real_pos>>32;
				msg.data[4] = motor[Motor1].real_pos>>24;
				msg.data[5] = motor[Motor1].real_pos>>16;
				msg.data[6] = motor[Motor1].real_pos>>8;
				msg.data[7] = motor[Motor1].real_pos;
				can_tx_enqueue(CAN_RM,msg);
				
				motor[Motor1].state &= ~FeedbackMask;
			}
			if((motor[Motor2].state&AliveMask)==AliveMask || (motor[Motor2].state&FeedbackMask)==FeedbackMask)
			{
				msg.length = 8;
				msg.data[0] = Motor2;
				msg.data[1] = motor[Motor2].state;
				msg.data[2] = raw_reading[Motor2].enc>>8;
				msg.data[3] = raw_reading[Motor2].enc&0xF;
				msg.data[4] = raw_reading[Motor2].vel>>8;
				msg.data[5] = raw_reading[Motor2].vel&0xF;
				msg.data[6] = raw_reading[Motor2].atq>>8;
				msg.data[7] = raw_reading[Motor2].atq&0xF;
				can_tx_enqueue(CAN_RM,msg);
				msg.length = 8;
				msg.data[0] = motor[Motor2].real_pos>>56;
				msg.data[1] = motor[Motor2].real_pos>>48;
				msg.data[2] = motor[Motor2].real_pos>>40;
				msg.data[3] = motor[Motor2].real_pos>>32;
				msg.data[4] = motor[Motor2].real_pos>>24;
				msg.data[5] = motor[Motor2].real_pos>>16;
				msg.data[6] = motor[Motor2].real_pos>>8;
				msg.data[7] = motor[Motor2].real_pos;
				can_tx_enqueue(CAN_RM,msg);
				
				motor[Motor2].state &= ~FeedbackMask;
			}
			if((motor[Motor3].state&AliveMask)==AliveMask || (motor[Motor3].state&FeedbackMask)==FeedbackMask)
			{
				msg.length = 8;
				msg.data[0] = Motor3;
				msg.data[1] = motor[Motor3].state;
				msg.data[2] = raw_reading[Motor3].enc>>8;
				msg.data[3] = raw_reading[Motor3].enc&0xF;
				msg.data[4] = raw_reading[Motor3].vel>>8;
				msg.data[5] = raw_reading[Motor3].vel&0xF;
				msg.data[6] = raw_reading[Motor3].atq>>8;
				msg.data[7] = raw_reading[Motor3].atq&0xF;
				can_tx_enqueue(CAN_RM,msg);
				msg.length = 8;
				msg.data[0] = motor[Motor3].real_pos>>56;
				msg.data[1] = motor[Motor3].real_pos>>48;
				msg.data[2] = motor[Motor3].real_pos>>40;
				msg.data[3] = motor[Motor3].real_pos>>32;
				msg.data[4] = motor[Motor3].real_pos>>24;
				msg.data[5] = motor[Motor3].real_pos>>16;
				msg.data[6] = motor[Motor3].real_pos>>8;
				msg.data[7] = motor[Motor3].real_pos;
				can_tx_enqueue(CAN_RM,msg);
				
				motor[Motor3].state &= ~FeedbackMask;
			}
			break;
		case FeedbackModeCnt:
			//NULL
			break;
	}
	last_motor_state[Motor1] = motor[Motor1].state;
	last_motor_state[Motor2] = motor[Motor2].state;
	last_motor_state[Motor3] = motor[Motor3].state;
}

void feedback_update(void)
{
	static uint32_t last_ticks = 0;
	if((get_ticks() - last_ticks)*FeedbackFreq >= 1000)
	{
		uart_motor_feedback_update();
		//can_mb_feedback_update();
		
		static uint8_t pressed = 0;
		if(pressed && !btn_pressed(Button1))
		{
			motor_selected = (MotorID)(((uint8_t)motor_selected + 1) % MotorCnt);
			pressed = 0;
		}
		else if(!pressed && btn_pressed(Button1))
			pressed = 1;
		
		alive_cnt++;
		if(alive_cnt <= CAN_MB_AliveTH)
		{
			static uint32_t last_led4_ticks = 0;
			if(get_ticks() - last_led4_ticks >= 250)
			{
				led_toggle(LED4);
				last_led4_ticks = get_ticks();
			}
		}
		else
			led_control(LED4,Bit_SET);
		last_ticks = get_ticks();
	}
}


#ifndef UART_ECHO
void feedback_motor_raw_reading(const uint8_t byte)
{
	//...
	switch(byte)
	{
		case '1':
			motor_selected = Motor1;
			break;
		case '2':
			motor_selected = Motor2;
			break;
		case '3':
			motor_selected = Motor3;
			break;
	}
}
void feedback_other_reading(const uint8_t byte)
{
	//...
}
#else
void uart1_interrupt_handler(const uint8_t byte)   {uart_tx_byte(COM1,byte);}
void uart3_interrupt_handler(const uint8_t byte)   {uart_tx_byte(COM3,byte);}
#endif

void can_mb_rx_handler(CanRxMsg* msg)
{
	alive_cnt = 0;
	uint32_t global_id  = (msg->StdId>>4)&0x1F;
	if(global_id >= ValkID*MotorCnt+MotorCnt || global_id < ValkID*MotorCnt)
		return;
	MotorID id = global_id % MotorCnt;
	CANcmd mode = (CANcmd)(msg->StdId & 0xF);
	int32_t hi;
	int32_t lo;
	lo = (((int32_t)msg->Data[0])<<24)
		 + (((int32_t)msg->Data[1])<<16)
		 + (((int32_t)msg->Data[2])<<8)
		 + ((int32_t)msg->Data[3]);
	hi = (((int32_t)msg->Data[4])<<24)
		 + (((int32_t)msg->Data[5])<<16)
		 + (((int32_t)msg->Data[6])<<8)
		 + ((int32_t)msg->Data[7]);
	switch(mode)
	{
		case Valk_Setting:
			if(msg->Data[0])
				valk_ctrl_mode = (ValkCtrlMode)msg->Data[0];
			if(msg->Data[1])
				can_feedback_mode = (CanFeedbackMode)msg->Data[1];
			if(msg->Data[2])
				motor[id].overheat = msg->Data[2];
			if(msg->Data[3] && (MotorType)msg->Data[3] != motor[id].mt)
				motor_init(id,(MotorType)msg->Data[3]);
			break;
		case Valk_Port:
			//...
			break;
		case Valk_Vel:
			if(hi)
				motor_set_vel(id,(int32_t)lo);
			else
				motor_set_vel_openloop(id,(int32_t)lo);
			break;
		case Valk_Pos:
			motor_set_pos(id,hi,lo);
			break;
		case Valk_MaxVel:
			motor[id].max_vel = lo;
			break;
		case Valk_Accel:
			motor[id].accel = lo;
			break;
		case Valk_pid_Kp:
			if(hi)
				motor[id].vel_pid.kp = lo;
			else
				motor[id].pos_pid.kp = lo;
			break;
		case Valk_pid_Ki:
			if(hi)
				motor[id].vel_pid.ki = lo;
			else
				motor[id].pos_pid.ki = lo;
			break;
		case Valk_pid_Kd:
			if(hi)
				motor[id].vel_pid.kd = lo;
			else
				motor[id].pos_pid.kd = lo;
			break;
		case Valk_pid_Max:
			if(hi)
				motor[id].vel_pid.max_output = lo;
			else
				motor[id].pos_pid.max_output = lo;
			break;
		case Valk_swerve_init:
			motor_swerve_init(hi,lo);
			break;
		case Valk_swerve_vel:
			motor_set_swerve(hi,lo);
			break;
		case Valk_feedback:
			if(msg->Data[0] < MotorCnt)
				motor[msg->Data[0]].state |= FeedbackMask;
			break;
		case Valk_NULL1:
		case Valk_NULL2:
			//NULL
			break;
		case Valk_CAN_CmdCnt:
			//NULL, wont happen
			break;
	}
}
void can_rm_rx_handler1(CanRxMsg* msg)
{
	raw_reading[Motor1].enc = msg->Data[0]*256+msg->Data[1];
	raw_reading[Motor1].vel = msg->Data[2]*256+msg->Data[3];
	raw_reading[Motor1].atq = msg->Data[4]*256+msg->Data[5];
	raw_reading[Motor1].tem = msg->Data[6];
	motor_control_independent(Motor1);
	motor[Motor1].alive_cnt = 0;
}
void can_rm_rx_handler2(CanRxMsg* msg)
{
	raw_reading[Motor2].enc = msg->Data[0]*256+msg->Data[1];
	raw_reading[Motor2].vel = msg->Data[2]*256+msg->Data[3];
	raw_reading[Motor2].atq = msg->Data[4]*256+msg->Data[5];
	raw_reading[Motor2].tem = msg->Data[6];
	motor_control_independent(Motor2);
	motor[Motor2].alive_cnt = 0;
}
void can_rm_rx_handler3(CanRxMsg* msg)
{
	raw_reading[Motor3].enc = msg->Data[0]*256+msg->Data[1];
	raw_reading[Motor3].vel = msg->Data[2]*256+msg->Data[3];
	raw_reading[Motor3].atq = msg->Data[4]*256+msg->Data[5];
	raw_reading[Motor3].tem = msg->Data[6];
	motor_control_independent(Motor3);
	motor[Motor3].alive_cnt = 0;
}

void valk_id_indicator(uint32_t ticks)
{
	static uint32_t start_ticks = 0;
	if(!start_ticks)
		start_ticks = ticks;
	if(ValkID<4)
	{
		if(ValkID>=0)
			led_control(LED1,Bit_RESET);
		if(ValkID>=1)
			led_control(LED2,Bit_RESET);
		if(ValkID>=2)
			led_control(LED3,Bit_RESET);
		if(ValkID>=3)
			led_control(LED4,Bit_RESET);
	}
	else
	{
		if(ValkID>=0)
			led_control(LED1,Bit_RESET);
		if(ValkID>=1)
			led_control(LED2,Bit_RESET);
		if(ValkID>=2)
			led_control(LED3,Bit_RESET);
		if(ValkID>=3)
			led_control(LED4,Bit_RESET);
	}
}
