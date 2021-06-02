#ifndef __FEEDBACK_H
#define __FEEDBACK_H

#include <stm32f4xx.h>
#include "define.h"
#include "uart.h"
#include "can_protocol.h"
#include "encoder.h"
#include "motor_control.h"

//#define UART_ECHO

//-setting-
//  uart
#define UART_BAUDRATE         115200
#define UART_Motor            COM1
#define UART_Other            COM3
#define UART_Motor_RxHandler  feedback_motor_raw_reading
#define UART_Other_RxHandler  feedback_other_reading

//  can
#define CAN_MB                CAN_1
#define CAN_MB_AliveTH        100
#define CAN_RM                CAN_2
#define CAN_MB_RxHandler      can_mb_rx_handler
#define CAN_RM_RxHandler1     can_rm_rx_handler1
#define CAN_RM_RxHandler2     can_rm_rx_handler2
#define CAN_RM_RxHandler3     can_rm_rx_handler3

#define CAN_MB_Mask           0x200
#define CAN_MB_RX_MASK        0x700
#define CAN_MB_FeedbackMask   0x199

#define CAN_RM_CtrlMask       0x200    //ranged from 0x200 to 37F
#define CAN_RM_RxMask1        0x201
#define CAN_RM_RxMask2        0x202
#define CAN_RM_RxMask3        0x203

typedef enum{
	//setting
	Valk_Setting,
	//	includes
	//	1 - valkyrie control mode
	//	2 - can feedback mode
	//	3 - overheat threshold
	//	4 - motor type
	
		//port
	Valk_Port,
	//	includes
	//	1 - encoder 1
	//	2 - encoder 2
	//	3 - pwm 1
	//	4 - pwm 2
	
	//ctrl
	Valk_Vel,
	Valk_Pos,
	Valk_MaxVel,
	Valk_Accel,
	
	//tuning
	Valk_pid_Kp,
	Valk_pid_Ki,
	Valk_pid_Kd,
	Valk_pid_Max,
	
	//swerve drive
	Valk_swerve_init,
	Valk_swerve_vel,
	
	//feedback request
	Valk_feedback,
	
	//not in use
	Valk_NULL1,
	Valk_NULL2,
	
	//total command count
	Valk_CAN_CmdCnt,
}CANcmd;

typedef struct{
	uint16_t enc;
	uint16_t vel;
	uint16_t atq;
	uint8_t  tem;
}MotorRawReadingStruct;

void feedback_init(void);

void feedback_update(void);

#ifndef UART_ECHO
void feedback_motor_raw_reading(const uint8_t byte);
void feedback_other_reading(const uint8_t byte);
#else
void uart1_interrupt_handler(const uint8_t byte);
void uart3_interrupt_handler(const uint8_t byte);
#endif

void can_mb_rx_handler(CanRxMsg* msg);
void can_rm_rx_handler1(CanRxMsg* msg);
void can_rm_rx_handler2(CanRxMsg* msg);
void can_rm_rx_handler3(CanRxMsg* msg);

void valk_id_indicator(uint32_t);

#endif
