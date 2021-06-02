#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "feedback.h"

typedef enum{
	Encoder1 = 0, 
	Encoder2,
	EncoderCount
}EncoderID;

typedef enum{
	ENC = 0,
	PWM,
}EncoderInputMode;

typedef struct{
	EncoderInputMode mode;
	int32_t r1;
	int32_t r2;
	//for ENC mode
	// r1 - encoder count
	// r2 - number of turns
	//for PWM mode
	// r1 - period
	// r2 - duty cycle
}EncoderStruct;

void encoder_1_init(EncoderInputMode mode);
void encoder_2_init(EncoderInputMode mode);

EncoderStruct get_encoder_reading(EncoderID id);

void encoder_1_update();
void encoder_2_update();

// Reset the encoder count
void reset_encoder(EncoderID id);

#endif
