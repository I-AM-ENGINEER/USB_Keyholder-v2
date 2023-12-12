#ifndef __BATTERY_H
#define __BATTERY_H

#include "main.h"

#define BATTERY_MCU_VOLTAGE_FILTER_ORDER    6
#define BATTERY_BAT_VOLTAGE_FILTER_ORDER    6

void battery_process( void );
uint16_t battery_get_voltage( void );
int8_t battery_get_charge_level( void );

#endif // !__BATTERY_H