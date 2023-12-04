#ifndef __SYSTEM_H__
#define __SYSTEM_H__


#include "main.h"

/* -------------------------------------------------------------------------- */

typedef enum{
	BTN_SW1_ID,
	BTN_SW2_ID,
	BTN_SW3_ID,
	BTN_SW4_ID,
	BTN_SW5_ID,
	BTN_SW6_ID,
	BTN_SW7_ID,
	BTN_SW8_ID,
	BTN_JPUSH_ID,
	BTN_JCCW_ID,
	BTN_JCW_ID,
	BTN_NONE,
} BTN_ids_t;

int exampleF(int number, char *string);
void power_GoToSleep( void );
void SYS_GoToSleep( void );
void power_SetNeedSleepFlag( void );
uint8_t power_GetNeedSleepFlag( void );
void SYS_UpdateButtons( void );
void SYS_ButtonsInit( void );
void SYS_init( void );

/* -------------------------------------------------------------------------- */

#endif // __SYSTEM_H__
