#ifndef _DEFINE_H
#define _DEFINE_H

#ifndef ARM_MATH_CM4
	#define ARM_MATH_CM4
#endif

//--define--
#define ControlFreq   1000
#define FeedbackFreq  50

#define AliveMask     0x01
#define ReachedMask   0x02
#define StallMask     0x04
#define OverloadMask  0x08
#define OverheatMask  0x10
#define CloseloopMask 0x20

#define AliveTH       50
#define StallTH       300

// rm3508 setting
#define RM3508_MaxVel 900000
#define RM3508_Accel  10000
/*
	Edited by Jonathan Chan 
	Date: July - 2019
	The PID contoroller we are using in Gungnir is 1DOF, first der term PID controller 
	For RM motor, 2DOF controller is used to avoid overshoot because "At least type 2" system with only 1DOF contorller must overshoot
	Actually, this is not really parallel form 2 DOF. I edited a bit according to the test. 
	read this: https://ww2.mathworks.cn/help/control/ug/two-degree-of-freedom-2-dof-pid-controllers.html
*/

//2DOF - Feedback Block
#define RM3508_VEL_KP 27000
#define RM3508_VEL_KI 70
#define RM3508_VEL_KD 5000
#define RM3508_VEL_KDD 0		//useless for this system 
////weighted r[n], y[n]
#define RM3508_VEL_Kb 1	//tested, coff=1 is best somehow 
#define RM3508_VEL_Kc 8	
//2DOF - Feedforward Block	--
#define RM3508_VEL_KP_FF 0
#define RM3508_VEL_KD_FF 0 
//Position target control 
#define RM3508_TORQUE 16384
#define RM3508_POS_KP 2000//2850
#define RM3508_POS_KI 0	//PD control is enough. dont add Kd. System will be crazy
#define RM3508_POS_KD 4500
#define RM3508_POS_ENABLE_THRS 50
#define RM3508_VEL    (RM3508_MaxVel)
#define RM3508_SCALED 10     //scaled 2^10 = 1024
#define RM3508_TEMP   60     //overheat threshold
#define RM3508_EncCnt 8192

#define RM3508_GearHi 3591
#define RM3508_GearLo 187

// rm2006 setting
#define RM2006_MaxVel 2000000
#define RM2006_Accel  20000

#define RM2006_VEL_KP 27000
#define RM2006_VEL_KI 700
#define RM2006_VEL_KD 1000
#define RM2006_VEL_KDD 0
//weighted e[n]
#define RM2006_VEL_Kb 1
#define RM2006_VEL_Kc 8	//weighted e[n], e[n-1]
//2DOF - Feedforward Block	--
#define RM2006_VEL_KP_FF 0	
#define RM2006_VEL_KD_FF 0 

//Position target control 
#define RM2006_TORQUE 10000
#define RM2006_POS_KP 0//2850
#define RM2006_POS_KI 0
#define RM2006_POS_KD 0//4500
#define RM2006_VEL    (RM2006_MaxVel)
#define RM2006_SCALED 10     //scaled 2^10 = 1024
#define RM2006_TEMP   60     //overheat threshold
#define RM2006_EncCnt 8192

#define RM2006_GearHi 36
#define RM2006_GearLo 1

// swerve drive setting
#define Swerve_GearHi 145
#define Swerve_GearLo 25
#define Swerve_VEL_KP 1
#define Swerve_MaxVel 250000


//--enum--
typedef enum{
	Valk1 = 0,
	Valk2,
	Valk3,
	Valk4,
	Valk5,
	Valk6,
	Valk7,
	Valk8,
	ValkCnt
}ValkyrieID;

typedef enum{
	Sentinel,
	Continuous,
	Passive,
	FeedbackModeCnt
}CanFeedbackMode;

typedef enum{
	Independent,
	Swerve,
	//Limb,
	ValkModeCnt
}ValkCtrlMode;

#endif 
