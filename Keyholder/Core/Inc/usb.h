#ifndef __USB_H__
#define __USB_H__

#include <stddef.h>
#include "stm32l1xx_hal.h"
//#include "stm32l1xx_hal_def.h"

/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */

#define USB_SUSPEND_MODE 	0x00
#define USB_HID_MODE			0x01
#define USB_CDC_MODE			0x02
#define USB_MSC_MODE			0x03
#define USB_DFU_MODE			0x04

/* -------------------------------------------------------------------------- */

void USB_keyboard_putc(char ch);
void USB_keyboard_puts(char *str, uint8_t length);

/* -------------------------------------------------------------------------- */

void USB_set_mode(uint8_t mode);
uint8_t USB_get_state( void );
void USB_main( void );
uint8_t get_USB_write_flag( void );
void set_USB_write_flag( uint8_t passwordNum );
uint8_t is_USB_connected( void );
void set_USB_connection( void );
void reset_USB_connection( void );
void wait_USB_insert_hotkey( void );

#endif // __USB_H__
