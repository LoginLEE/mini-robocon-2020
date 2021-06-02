#ifndef __TICKS_H
#define __TICKS_H

#include "stm32f4xx.h"

extern volatile uint32_t msec;

/**
  * @brief  Initialization of ticks timer
  * @param  None
  * @retval None
  */
void ticks_init(void);

/**
  * @brief  Return no of milliseconds passed
  * @param  None
  * @retval Milliseconds passed
  */
#define get_ticks() msec

u32 get_subticks(void);

/**
  * @brief  Reset the ticks timer
  * @param  None
  * @retval None
  */
void ticks_reset(void);

/**
  * @brief  Wait fot the time specified by the user
  * @param  ms Amount of time in ms
  * @retval None
  */
void delay(u32 ms);

#endif
