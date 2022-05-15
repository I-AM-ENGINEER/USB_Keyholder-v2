#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include <stddef.h>
#include "stm32l1xx_hal.h"

/* -------------------------------------------------------------------------- */

void UI_print_menu( void );
void setPushedButton( void );
void setDisplayUpdateFlag( void );

extern uint8_t currentTab;

/* -------------------------------------------------------------------------- */

#endif // __USER_INTERFACE_H__
