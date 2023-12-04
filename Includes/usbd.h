#ifndef __USBD_H__
#define __USBD_H__

typedef enum {
	USB_STATUS_CONNECTED,
	USB_STATUS_DISCONNECTED,
} USB_status_t;

void USB_keyboard_puts(const char* str);
void USB_keyboard_putc(char ch);

void USB_update_status( void );
USB_status_t USB_connection_status( void );

#endif //__USB_H__
