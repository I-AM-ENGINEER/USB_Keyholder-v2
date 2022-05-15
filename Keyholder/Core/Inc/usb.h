#ifndef __USB_H__
#define __USB_H__

#include <stddef.h>
#include "stm32l1xx_hal.h"

/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */

void usb_keyboard_putc(char ch);
void usb_keyboard_puts(char *str, uint8_t length);

/* -------------------------------------------------------------------------- */

uint8_t get_USB_write_flag( void );
void set_USB_write_flag( uint8_t passwordNum );
uint8_t is_USB_connected( void );
void set_USB_connection( void );
void reset_USB_connection( void );
void wait_USB_insert_hotkey( void );

#endif // __USB_H__
