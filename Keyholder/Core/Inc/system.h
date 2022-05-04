#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "stm32l1xx_hal.h"

/* -------------------------------------------------------------------------- */

int exampleF(int number, char *string);
void power_GoToSleep( void );
void power_SetNeedSleepFlag( void );
uint8_t power_GetNeedSleepFlag( void );

/* -------------------------------------------------------------------------- */

#endif // __SYSTEM_H__
