#ifndef _MOTOR_CONTROL_H
#define _MOTOR_CONTROL_H

#include <stm32f4xx.h>
#include "define.h"
#include "ticks.h"
#include "macro_math.h"
#include "approx_math.h"
#include "trigon_math.h"
#include "can_protocol.h"
#include "encoder.h"
#include "feedback.h"
//#include "can_motor.h"
#include "led.h"
#include "button.h"

typedef enum {
	MOTOR_1 = 0,
	MOTOR_2,
	MOTOR_3,
	MOTOR_4,
	MOTOR_5,
	MOTOR_6,
	MOTOR_7,
	MOTOR_8,
	MOTOR_9,
	MOTOR_10,
	MOTOR_11,
	MOTOR_12,
	MOTOR_13,
	MOTOR_14,
	MOTOR_15,
	MOTOR_16
} gun_MotorID;

typedef enum{
	Motor1 = 0,
	Motor2,
	Motor3,
	MotorCnt
}MotorID;

typedef enum{
	RM3508,
	RM2006
}MotorType;

typedef enum{
	Closeloop = 0,
	Openloop
}CloseLoopFlag;

typedef struct{
	int32_t err;
	int32_t c_err;
	int32_t p_err;
	int32_t kp;
	int32_t ki;
	int32_t kd;
	int32_t kb;
	int32_t kc;
	int32_t max_output;
	int32_t scaled_up;
}PID_Struct;

typedef struct{
	uint8_t state;
	uint8_t overheat;
	uint32_t alive_cnt;
	uint32_t stall_cnt;
	MotorType mt;
	int32_t torque;
	uint32_t enc_cnt;
	int64_t real_pos;
	int64_t cur_pos;
	int64_t tar_pos;
	int32_t real_vel;
	int32_t cur_vel;
	int32_t tar_vel;
	int32_t vel_cnt;
	int32_t max_vel;
	int32_t accel;
	PID_Struct pos_pid;
	PID_Struct vel_pid;
	int32_t rv;
	int32_t cv;
	//void (*motor_control)(void);
}MotorStruct;

typedef struct{
	bool flipped;
	EncoderID enc_used;
	int32_t a;
	int32_t v;
	int32_t offset;
}SwerveStruct;

//init
void motor_basic_init(MotorID);
void motor_init(MotorID,MotorType);
void motor_swerve_init(uint8_t,int32_t);

//path control
void motor_set_vel_openloop(MotorID,int16_t);
void motor_set_vel(MotorID,int32_t);
void motor_set_pos(MotorID,int32_t,int32_t);
void motor_set_swerve(int32_t,int32_t);
void motor_set_accel(MotorID,int32_t);
int64_t motor_get_pos(MotorID);

//pid control
void motor_control_independent(MotorID);
void motor_control_steering();

//update
void motor_status_update(void);

#endif
